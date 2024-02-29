# Sets the base image for subsequent instructions
ARG ARG_BUILD_FROM="ghcr.io/entelecheia/course-template:latest-base"
FROM $ARG_BUILD_FROM

# Setting ARGs and ENVs for user creation and workspace setup
ARG ARG_USERNAME="dev"
ARG ARG_USER_UID=9001
ARG ARG_USER_GID=$ARG_USER_UID
ARG ARG_WORKSPACE_ROOT="/workspace"
ENV USERNAME $ARG_USERNAME
ENV USER_UID $ARG_USER_UID
ENV USER_GID $ARG_USER_GID
ENV WORKSPACE_ROOT $ARG_WORKSPACE_ROOT

# Sets up the workspace for the user
RUN if [ ! -d $WORKSPACE_ROOT/projects ]; then mkdir -p $WORKSPACE_ROOT/projects; fi

# Setting ARGs and ENVs for the app
ARG ARG_APP_SOURCE_REPO="entelecheia/course-template"
ARG ARG_APP_INSTALL_ROOT="/workspace/projects"
ARG ARG_APP_DIRNAME="course_temp"
ARG ARG_APP_SOURCE_BRANCH="main"
ARG ARG_APP_SERVICE_NAME="app"
ENV APP_SOURCE_REPO $ARG_APP_SOURCE_REPO
ENV APP_INSTALL_ROOT $ARG_APP_INSTALL_ROOT
ENV APP_DIRNAME $ARG_APP_DIRNAME
ENV APP_SOURCE_BRANCH $ARG_APP_SOURCE_BRANCH
ENV APP_SERVICE_NAME $ARG_APP_SERVICE_NAME
ENV APP_SRC_DIR=${APP_INSTALL_ROOT}/${APP_DIRNAME}
ENV APP_VIRTUAL_ENV=${APP_INSTALL_ROOT}/.venvs/${APP_DIRNAME}
ENV APP_WORKSPACE_ROOT=${APP_INSTALL_ROOT}/workspace

# Clones the app repository from GitHub
RUN git clone --branch $APP_SOURCE_BRANCH https://github.com/${ARG_APP_SOURCE_REPO}.git ${APP_SRC_DIR} &&\
    cd ${APP_SRC_DIR} &&\
    git checkout $APP_SOURCE_BRANCH

# Sets the working directory to workspace root
WORKDIR $WORKSPACE_ROOT
# Copies scripts from host into the image
COPY ./.docker/scripts/ ./scripts/



# Creates a non-root user with sudo privileges
# check if user exists and if not, create user
RUN if id -u $USERNAME >/dev/null 2>&1; then \
    echo "User exists"; \
    else \
    groupadd --gid $USER_GID $USERNAME && \
    adduser --uid $USER_UID --gid $USER_GID --force-badname --disabled-password --gecos "" $USERNAME && \
    echo "$USERNAME:$USERNAME" | chpasswd && \
    adduser $USERNAME sudo && \
    echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers && \
    echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME; \
    fi

RUN chown -R $USERNAME:$USERNAME $WORKSPACE_ROOT
RUN chown -R $USERNAME:$USERNAME $APP_INSTALL_ROOT

# Switches to the newly created user
USER $USERNAME

# Install dotfiles
ARG ARG_USER_FULLNAME
ARG ARG_USER_EMAIL
ARG ARG_GITHUB_USERNAME
ARG ARG_SYSTEM_HOSTNAME
ARG ARG_WORKSPACE_LOCATION="/"
ARG ARG_DOTFILES_VERSION="0.1.0"
ENV USER_FULLNAME $ARG_USER_FULLNAME
ENV USER_EMAIL $ARG_USER_EMAIL
ENV GITHUB_USERNAME $ARG_GITHUB_USERNAME
ENV SYSTEM_HOSTNAME $ARG_SYSTEM_HOSTNAME
ENV WORKSPACE_LOCATION $ARG_WORKSPACE_LOCATION
ENV DOTFILES_VERSION $ARG_DOTFILES_VERSION
ENV DOTFILES_APPLY_ROOTMOI=0
ENV DOTFILES_USE_CODE=1
ENV DOTFILES_USE_PYTHON_TOOLS=1
ENV DOTFILES_MINIMUM=1
ENV REMOTE_CONTAINERS=1

RUN echo "Current user: $USERNAME"
RUN echo "Dotfiles version: $DOTFILES_VERSION"
RUN if [ -d "/home/$USERNAME/.dotfiles" ]; then \
    echo "Dotfiles already installed"; \
    else \
    sh -c "$(wget -qO- https://dotfiles.entelecheia.ai/install)"; \
    fi

USER root
RUN usermod -u "${USER_UID}" "${USERNAME}"
RUN groupmod -g "${USER_GID}" "${USERNAME}"
RUN chown --recursive "${USER_UID}:${USER_GID}" "${WORKSPACE_ROOT}"
RUN chown --recursive "${USER_UID}:${USER_GID}" "${APP_INSTALL_ROOT}"
USER $USERNAME

# Specifies the command that will be executed when the container is run
CMD ["bash"]
