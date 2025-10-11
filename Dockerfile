## Multi-stage Dockerfile: build C binaries with gcc then copy to slim runtime
FROM gcc:12 as builder
WORKDIR /usr/src/app

# Copy project files
COPY . .

# Build server and client
RUN gcc -fdiagnostics-color=always -g connect_4.c util/hash_map.c server.c -o /usr/src/app/server \
 && gcc -fdiagnostics-color=always -g client.c -o /usr/src/app/client || true

FROM debian:bookworm-slim
WORKDIR /app

# Install minimal runtime deps (optional)
RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates && rm -rf /var/lib/apt/lists/*

# Copy built binaries from builder
COPY --from=builder /usr/src/app/server /app/server
COPY --from=builder /usr/src/app/client /app/client

EXPOSE 12345

# Default to running the server
CMD ["/app/server"]
