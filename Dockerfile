FROM devkitpro/devkitarm:20260610

LABEL author="Striaton Lab Team"
LABEL name="Poke Transporter GB"

# uid and gid are overwritten by build command
ARG USER=docker
ARG UID
ARG GID
RUN groupadd -g $GID $USER
RUN useradd -m -u ${UID} -g ${GID} ${USER}

ENV DEBIAN_FRONTEND="noninteractive"
ENV IS_PTGB=true

RUN apt-get update && apt-get install -y sudo build-essential cmake bison flex pkg-config libpng-dev git python3-pip jq && pip install --root-user-action=ignore pandas requests openpyxl debugpy pypng --break-system-packages

RUN echo "${USER} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USER} && chmod 0440 /etc/sudoers.d/${USER}

RUN mkdir -p rgbds && curl -fsSL https://github.com/gbdev/rgbds/releases/download/v1.0.2+hotfix/rgbds-linux-x86_64.tar.xz | tar -xJ -C rgbds && cd rgbds && ./install.sh
RUN mkdir -p flipsdir && cd flipsdir \
    && curl -fsSL https://github.com/Alcaro/Flips/archive/refs/tags/v198.tar.gz | tar -xz --strip-components=1 \
    && TARGET=cli ./make-linux.sh --profile=no \
    && cp flips /usr/local/bin \
    && cd .. && rm -rf flipsdir

RUN git config --global --add safe.directory /ptgb

USER ${UID}:${GID}
WORKDIR /home/${USER}
