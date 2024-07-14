import argparse
import requests
import matplotlib.pyplot as plt
import seaborn as sns
import os
import pandas as pd
from collections import defaultdict
from collections import OrderedDict
from datetime import datetime


def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate graphs and markdown reports of GitHub commit contributions."
    )
    parser.add_argument(
        "--owner",
        type=str,
        default=os.getenv("GITHUB_OWNER"),
        help="Owner of the GitHub repository",
    )
    parser.add_argument(
        "--repo",
        type=str,
        default=os.getenv("GITHUB_REPO"),
        help="GitHub repository name",
    )
    parser.add_argument(
        "--exclude-ids",
        type=str,
        default=os.getenv("CONTRIBUTION_EXCLUDE_GITHUB_IDS", ""),
        help="Comma-separated list of GitHub IDs to exclude from the contribution graph",
    )
    parser.add_argument(
        "--start-date",
        type=str,
        default=os.getenv("CONTRIBUTION_START_DATE"),
        help="Start date in YYYY-MM-DD format",
    )
    parser.add_argument(
        "--end-date",
        type=str,
        default=os.getenv("CONTRIBUTION_END_DATE"),
        help="End date in YYYY-MM-DD format",
    )
    parser.add_argument(
        "--frequency",
        type=str,
        default=os.getenv("CONTRIBUTION_FREQUENCY", "W"),
        choices=["W", "M"],
        help="Frequency of contributions: W for weekly, M for monthly",
    )
    parser.add_argument(
        "--figure-path",
        type=str,
        default=os.getenv("CONTRIBUTION_FIGURE_PATH", "book/about/contributors.png"),
        help="File path for the output graph image",
    )
    parser.add_argument(
        "--markdown-path",
        type=str,
        default=os.getenv("CONTRIBUTION_MARKDOWN_PATH", "book/about/contributors.md"),
        help="File path for the output Markdown document",
    )
    parser.add_argument(
        "--token",
        type=str,
        default=os.getenv("GITHUB_TOKEN"),
        help="GitHub personal access token",
    )
    return parser.parse_args()


def fetch_commits(owner, repo, start_date, end_date, token):
    url = f"https://api.github.com/repos/{owner}/{repo}/commits"
    headers = {"Authorization": f"token {token}"}
    params = {"since": start_date, "until": end_date, "per_page": 100}
    commits = []
    while url:
        response = requests.get(url, headers=headers, params=params)
        commits.extend(response.json())
        url = response.links.get("next", {}).get("url", None)
    return commits


def process_commits(commits, frequency, excluded_ids):
    contributions = defaultdict(lambda: defaultdict(int))
    for commit in commits:
        if "author" in commit and commit["author"] is not None:
            author_id = commit["author"]["login"]
            if author_id not in excluded_ids:
                if commit.get("commit") and commit["commit"].get("author"):
                    date_str = commit["commit"]["author"]["date"]
                    date = datetime.strptime(date_str, "%Y-%m-%dT%H:%M:%SZ")
                    if frequency == "W":
                        period = f"W{date.isocalendar()[1]:02d}"
                    else:
                        period = date.strftime("%Y-%m")
                    contributions[period][author_id] += 1
    sorted_periods = sorted(contributions.keys())
    sorted_contributions = OrderedDict(
        (period, contributions[period]) for period in sorted_periods
    )
    return sorted_contributions


def save_chart(contributions, figure_path):
    df = pd.DataFrame(contributions).fillna(0)
    plt.figure(figsize=(12, 8))
    sns.heatmap(
        df, annot=True, fmt=".0f", cmap="Blues", cbar_kws={"label": "Number of Commits"}
    )
    plt.title("Commit Contributions Heatmap")
    plt.ylabel("Time Period")
    plt.xlabel("Contributors (GitHub ID)")
    plt.savefig(figure_path)


def generate_markdown(contributions, figure_path, markdown_path):
    df = pd.DataFrame(contributions).fillna(0)
    markdown_table = df.to_markdown()

    with open(markdown_path, "w") as md_file:
        md_file.write("# Contributors\n\n")
        md_file.write(
            "We would also like to thank all the students who have provided feedback and suggestions to improve this project.\n\n"
        )
        md_file.write("## Weekly Contributions\n\n")
        md_file.write(markdown_table + "\n\n")
        md_file.write(
            f"![Weekly Contributions Chart]({os.path.basename(figure_path)})\n"
        )


def main():
    args = parse_args()
    commits = fetch_commits(
        args.owner, args.repo, args.start_date, args.end_date, args.token
    )
    excluded_ids = set(args.exclude_ids.split(",")) if args.exclude_ids else set()
    contributions = process_commits(commits, args.frequency, excluded_ids)
    save_chart(contributions, args.figure_path)
    generate_markdown(contributions, args.figure_path, args.markdown_path)


if __name__ == "__main__":
    main()
