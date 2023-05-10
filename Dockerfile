FROM gcc:12.2.0

WORKDIR /app

RUN apt-get update && \
    apt-get install -y build-essential gdb cmake ninja-build curl && \
    rm -rf /var/lib/apt/lists/*

RUN curl -fsSL https://code-server.dev/install.sh | sh

RUN code-server --install-extension ms-vscode.cpptools

COPY main.c .

RUN gcc -o app main.c

EXPOSE 8080

CMD ["code-server", "--bind-addr", "0.0.0.0:8080", "--auth", "none"]
