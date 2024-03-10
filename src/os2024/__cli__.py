"""Command line interface for os2024"""

# Importing the libraries
import os
import subprocess
import webbrowser

import click
import yaml

from os2024._version import __version__

__package_path__ = os.path.abspath(os.path.dirname(__file__))
__package_name__ = os.path.basename(__package_path__)


def load_about() -> dict:
    """
    Load the about.yml file.
    """

    about_path = os.path.join(__package_path__, f"conf/about/{__package_name__}.yaml")
    if not os.path.isfile(about_path):
        click.echo(f"The `{about_path}` was not found.")
        raise click.Abort()
    with open(about_path, "r", encoding="utf-8") as f:
        return yaml.load(f, Loader=yaml.Loader)


__about__ = load_about()


@click.command()
@click.version_option(__version__)
@click.option("--build", "-b", is_flag=True, help="Build the book")
@click.option("--local", "-l", is_flag=True, help="Open the book html built locally")
def main(build: bool = False, local: bool = False) -> None:
    """
    This is the cli function of the package.
    It opens the book in the browser or builds the book.
    """
    if build:
        click.echo("Building the book...")
        build_book()
    else:
        open_book(local)


def open_book(local: bool = False) -> None:
    """
    Open the book in the browser.
    """
    if local:
        html_file: str = "book/_build/html/index.html"
        html_path = os.path.join(__package_path__, html_file)
        # check if the book is built
        if not os.path.isfile(html_path):
            click.echo(f"The `{html_path}` was not found.")
            click.echo(
                f"The book is not built yet. Please run '{__package_name__} build' first."
            )
            return
        click.echo("Opening the book locally...")
        webbrowser.open_new_tab(f"file://{html_path}")
    else:
        click.echo("Opening the book...")
        webbrowser.open_new_tab(__about__["homepage"])


def build_book() -> None:
    """
    Build the book.
    """
    book_path = os.path.join(__package_path__, "book")
    if not os.path.isdir(book_path):
        click.echo("The book directory was not found.")
        return
    subprocess.run(["jupyter-book", "build", book_path], check=True)


# main function for the main module
if __name__ == "__main__":
    main()
