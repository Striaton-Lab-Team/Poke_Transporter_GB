SHELL = /bin/sh
SRCDIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PYPACKAGES := debugpy pandas pypng requests openpyxl
BUILD_LANGS := japanese english french german italian spanishEU spanishLA korean chineseSI chineseTR portugueseBR
BUILD_TYPES := release debug
BUILD_XLSXS := remote local

UID := $(shell id -u)
GID := $(shell id -g)
export UID
export GID

.ONESHELL:

default: help

.PHONY: help
help: # Display this help message
	@tabs 2
	@printf "\033[1;4mTargets\033[0m\n"	
# shows help for all targets that follow the format of "target: #", where target includes a-z, A-Z, 0-9, and _ characters
	@grep -E '^[a-zA-Z0-9_]+: #'  Makefile | while read -r l; do printf "\t\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:$$(echo $$l | cut -f 2- -d'#')\n"; done

# if [ $(id | grep -c docker) = 0 ]; then
# $1 = command name
# $2 = package name
define check_package
	@if [ "$(2)" = "docker" ]; then
		if ! command -v $(1) >/dev/null 2>&1; then
			printf "\033[1;31mDocker is not installed!\n"
			printf "Please install Docker through your package manager.\033[0m\n"
			exit
		elif [ "$$(uname)" = "Darwin" ]; then
			echo "macOS detected; skipping group check."
		elif [ "$$(id | grep -c docker)" = 0 ]; then
			printf "\033[1;31mUser is not in docker group!\033[0m\n"
			if [ "$$(cat /etc/group | grep -c docker)" = 0 ]; then
				echo "Creating docker group..."
				sudo groupadd docker
			fi
			echo "Adding user to docker group..."
			sudo usermod -aG docker $$USER
			echo
			printf "\033[1;31mPlease log out and log back in for this to take effect.\033[0m\n"
			exit
		else
			echo "Docker is installed and user is in docker group."
		fi
	else
		if ! command -v $(1) >/dev/null 2>&1; then
			printf "\033[1;31m$(2) not found. Installing...\033[0m\n"
			if [ "$(2)" = "rgbds" ]; then
				mkdir -p rgbds && curl -L https://github.com/gbdev/rgbds/releases/download/v1.0.2+hotfix/rgbds-linux-x86_64.tar.xz | tar -xJ -C rgbds && cd rgbds && ./install.sh && cd .. && rm -rf rgbds
			else
				apt update && apt install -y $(1)
			fi
		else
			echo "$(2) found."
		fi
	fi
endef

.PHONY: setup
setup: # Setup all required dependencies
	@if [ -f /.dockerenv ] || [ -n "$$GITPOD" ]; then
		if [ -z "$$IS_PTGB" ]; then
			printf "\033[1;31mRunning inside an non-Poké Transporter GB Docker shell!\n"
			printf "Please run this on the host machine or inside the Poké Transporter GB's Docker shell.\033[0m"
			exit
		else
			echo "Running in Docker shell; continuing..."
		fi
	else
		echo "Running outside of Docker shell. Checking Docker installation..."
		$(call check_package,docker,docker)
		if [ "$$(docker container ls -f "name=ptgb" --format "{{.Names}}")" = "ptgb" ]; then
			echo "Docker container found and running."
		elif [ "$$(docker container ls -af "name=ptgb" --format "{{.Names}}")" = "ptgb" ]; then
			echo "Docker container found; starting..."
			docker start ptgb >/dev/null
		else
			echo "Docker container does not exist."
			echo "Checking status of docker buildx..."
			if docker buildx version >/dev/null 2>&1; then
				echo "buildx installed. Continuing..."
			else
				printf "\033[1;31mbuildx is not installed!\n"
				printf "\033[1;31mPlease install through your package manager.\n"
				exit
			fi
			echo "Creating Docker image..."
			mkdir -p $(SRCDIR)/docker-build && cd $(SRCDIR)/docker-build
			DOCKER_BUILDKIT=1 docker build --platform linux/amd64 -t ptgb-builder:latest --build-arg UID=$$UID --build-arg GID=$$GID -f $(SRCDIR)/Dockerfile $(SRCDIR)/docker-build
			rm -rf $(SRCDIR)/docker-build
			echo
			echo "Creating Docker container..."
			docker container create --platform linux/amd64 -w /ptgb -v $(SRCDIR):/ptgb --name ptgb -it ptgb-builder:latest
			echo "Starting Docker container..."
			docker start ptgb >/dev/null
		fi
		echo
		docker exec ptgb make --no-print-directory -C /ptgb _setup
		echo
		printf "\033[1;32mEverything is set up!\033[0m\n"
	fi

.PHONY: _setup
_setup:
	@echo "Checking Docker container's packages..."
# Python
	$(call check_package,python3,python3)
	
	for package in $(PYPACKAGES); do
		if pip show "$$package" > /dev/null 2>&1; then
			echo "$$package found."
		else
			echo "Installing $$package"
			pip install --root-user-action=ignore "$$package"
		fi
	done

# rgbds
	$(call check_package,rgbasm,rgbds)
# jq
	$(call check_package,jq,jq)

# $1 = command name
# $2 = package name
define check_package_host
	@if ! command -v $(1) >/dev/null 2>&1; then
		printf "\033[1;31m$(2) is not installed!\n"
		printf "Please install $(2) through your package manager.\033[0m\n"
		exit
	fi
endef

.PHONY: setup_no_docker
# hidden legacy build option
setup_no_docker:
	$(call check_package_host,python3,python3)
	@if [ ! -e "$(SRCDIR)/.venv/bin/activate" ]; then
		echo "venv not found. Creating venv..."
		python3 -m venv .venv
	fi
	@if [ -z "$$VIRTUAL_ENV" ]; then
		echo "Activating venv..."
		source $(SRCDIR)/.venv/bin/activate
	fi

	@for package in $(PYPACKAGES); do
		if pip show "$$package" > /dev/null 2>&1; then
			echo "$$package found."
		else
			echo "$$package is not installed. Installing..."
			pip install "$$package"
		fi
	done
	$(call check_package_host,rgbasm,rgbds)

	@printf "\033[1;32mEverything is set up!\033[0m\n"

.PHONY: configure
configure: # Specify ROM Language, ROM Type, and Text Source
	$(call check_package_host,dialog,dialog)
	$(call check_package_host,jq,jq)
	
# generate default options.json (based on makefile vars) if it doesn't already exist
	@touch $(SRCDIR)/options.json

	set -- $(BUILD_LANGS)
	langs=$$(printf '%s\n' "$$@" | jq -R -s 'split("\n")[:-1]')

	set -- $(BUILD_TYPES)
	types=$$(printf '%s\n' "$$@" | jq -R -s 'split("\n")[:-1]')

	set -- $(BUILD_XLSXS)
	xlsxs=$$(printf '%s\n' "$$@" | jq -R -s 'split("\n")[:-1]')

	jq -n \
		--argjson langs "$$langs" \
		--argjson types "$$types" \
		--argjson xlsxs "$$xlsxs" \
		'{
			BUILD_LANGS: $$langs,
			BUILD_TYPES: $$types,
			BUILD_XLSXS: $$xlsxs,
			selected: {
				lang: "",
				type: "",
				xlsx: ""
			}
		}' > "$(SRCDIR)/options.json"

	menu() {
		title="$$1"
		json="$$2"
		array_key="$$3"

		menu_args=""
		i=1

		while IFS= read -r item; do
			menu_args="$$menu_args $$i $$item"
			i=$$((i + 1))
		done <<-EOF
			$$(jq -r ".$${array_key}[]" "$$json")
		EOF
		choice=$$(dialog --stdout \
			--menu "$$title" 0 0 15 \
			$$menu_args)

		status=$$?

		case $$status in
			0)
				jq -r ".$${array_key}[$$((choice - 1))]" "$$json"
				return 0
				;;
			1)
				printf "\033[1;31mUser canceled dialog.\033[0m\n" >&2
				return 1
				;;
			255)
				printf "\033[1;31mUser pressed escape.\033[0m\n" >&2
				return 1
				;;
			*)
				printf "\033[1;31mUnknown error occurred.\033[0m\n" >&2
				return 1
				;;
		esac
	}

# prompt user for selections
	if ! LANG=$$(menu "ROM Language" options.json "BUILD_LANGS"); then
		exit 1
	fi
	if ! TYPE=$$(menu "ROM Type" options.json "BUILD_TYPES"); then
		exit 1
	fi
	if ! XLSX=$$(menu "Text Source" options.json "BUILD_XLSXS"); then
		exit 1
	fi	
	clear
	tmp=$$(mktemp)
# store selections in options.json
	jq \
		--arg lang "$$LANG" \
		--arg type "$$TYPE" \
		--arg xlsx "$$XLSX" \
		'.selected.lang = $$lang | .selected.type = $$type | .selected.xlsx = $$xlsx' \
		$(SRCDIR)/options.json > "$$tmp" && mv "$$tmp" $(SRCDIR)/options.json

	@printf "\033[1;32mBuild configured!\033[0m\n"

.PHONY: clean
clean: # Cleans the build directory
	@echo "Starting Docker container..."
	@docker start ptgb >/dev/null
	@docker exec ptgb make -f /ptgb/container.mk -C /ptgb SRCDIR="/ptgb" PTGB_PROJECT_NAME="$(notdir $(SRCDIR))" clean

.PHONY: clean_no_docker
# hidden legacy build option
clean_no_docker:
	@echo "Cleaning..."
	make -f $(SRCDIR)/container.mk clean SRCDIR="$(SRCDIR)"

.PHONY: build
build: # Builds the ROM
	@echo "Starting Docker container..."
	@docker start ptgb >/dev/null
	@docker exec ptgb make -f /ptgb/container.mk -C /ptgb SRCDIR="/ptgb" PTGB_PROJECT_NAME="$(notdir $(SRCDIR))" all

.PHONY: clean_no_docker
# hidden legacy build option
build_no_docker:
	@if [ -z "$$VIRTUAL_ENV" ]; then
		echo "Activating venv..."
		source $(SRCDIR)/.venv/bin/activate
	fi
	@echo "Building..."
	make -f $(SRCDIR)/container.mk all SRCDIR="$(SRCDIR)"
