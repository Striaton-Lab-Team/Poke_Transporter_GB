## Prerequisites
### Windows
1. Install [Docker Desktop](https://docs.docker.com/desktop/setup/install/windows-install/).
2. Install a [WSL 2 distro](https://learn.microsoft.com/en-us/windows/wsl/install).
3. Enable Docker integration with your WSL distro:

	<img src="https://i.imgur.com/sd0epzF.png" width="500" height="300"/>

4. Install [git](https://git-scm.com/install/windows).
5. Open a WSL shell and install `jq` and `dialog` through your package manager.
6. Continue with [Cloning the Repository](#cloning-the-repository).

> *Make sure to stay inside the WSL shell for the rest of this tutorial!*

### Linux
1. Install [Docker Engine](https://docs.docker.com/engine/install)
2. Install `jq`, `dialog`, and `git` through your package manager.
3. Continue with [Cloning the Repository](#cloning-the-repository).
### macOS
1. Install [Docker Desktop](https://docs.docker.com/desktop/setup/install/mac-install/).
2. Install `gmake`, `jq`, `dialog`, and `git` through [Homebrew](https://brew.sh/).
3. Continue with [Cloning the Repository](#cloning-the-repository).
## Cloning the Repository
- `cd` to the directory you want to clone the repository:
`cd ~/path/where/repo/will/be/cloned/`

> The repository folder will be stored in this directory (i.e. `~/path/where/repo/will/be/cloned/Poke_Transporter_GB`)

- Clone the repository and enter its directory:
`git clone https://github.com/Striaton-Lab-Team/Poke_Transporter_GB.git`

- Update all submodules:
`git submodule update --init --recursive`

- Continue with [Building](#building-1)
## Building
> If you are on macOS, use `gmake` instead of `make`!
1. View available targets with `make help`:
```
Targets
help: Display this help message
setup: Setup all required dependencies
configure: Specify ROM Language, ROM Type, and Text Source
clean: Cleans the build directory
build: Builds the ROM
```
2. Setup the build environment with  `make setup`.
3. Configure which ROM to build with `make configure`.
4. Build the ROM with `make build`.

