"""Command line interface for os2024"""

# Importing the libraries
import os
import subprocess
import webbrowser

import click

from os2024._version import __version__


@click.command()
@click.version_option(__version__)
@click.option("--build", "-b", is_flag=True, help="Build the book")
@click.option("--local", "-l", is_flag=True, help="Open the book html built locally")
def main(build: bool = False, local: bool = False) -> None:
    """
    This is the cli function of the package.
    It opens the book in the browser or builds the book.

    Args:
        build (bool, optional): Build the book. Defaults to False.
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
        html_path = os.path.abspath(html_file)
        # check if the book is built
        if not os.path.isfile(html_path):
            click.echo("The book is not built yet. Please run 'os2024 build' first.")
            return
        click.echo("Opening the book locally...")
        webbrowser.open_new_tab(f"file://{html_path}")
    else:
        click.echo("Opening the book...")
        webbrowser.open_new_tab("https://os2024.halla.ai/")


def build_book() -> None:
    """
    Build the book.
    """
    book_path = os.path.abspath("book")
    if not os.path.isdir(book_path):
        click.echo("The book directory was not found.")
        return
    subprocess.run(["jupyter-book", "build", "book"], check=True)


# main function for the main module
if __name__ == "__main__":
    main()
