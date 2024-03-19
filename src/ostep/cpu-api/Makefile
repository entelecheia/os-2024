# To do stuff with make, you type `make` in a directory that has a file called
# "Makefile". You can also type `make -f <makefile>` to use a different filename.
#
# A Makefile is a collection of rules. Each rule is a recipe to do a specific
# thing, sort of like a grunt task or an npm package.json script.
#
# A rule looks like this:
#
# <target>: <prerequisites...>
#   <commands>
#
# The "target" is required. The prerequisites are optional, and the commands
# are also optional, but you have to have one or the other.
#
# Type `make` to show the available targets and a description of each.
#

.DEFAULT_GOAL := help

.PHONY: help
help: ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n make \033[36m<target>\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ { printf " \033[36m%-25s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

##@ Build

all: p1 p2 p3 p4 ## Build all targets

p1: p1.c ## Build target p1
	gcc -o p1 p1.c -Wall

p2: p2.c ## Build target p2
	gcc -o p2 p2.c -Wall

p3: p3.c ## Build target p3
	gcc -o p3 p3.c -Wall

p4: p4.c ## Build target p4
	gcc -o p4 p4.c -Wall

##@ Clean-up

clean: ## Remove all build artifacts
	rm -f p1 p2 p3 p4
