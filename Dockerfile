FROM gcc:12 as builder
WORKDIR /usr/src/app

COPY . .

RUN gcc -fdiagnostics-color=always -g connect_4.c util/hash_map.c server.c -o /usr/src/app/server \
 && gcc -fdiagnostics-color=always -g client.c -o /usr/src/app/client || true

FROM debian:bookworm-slim
WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates && rm -rf /var/lib/apt/lists/*

COPY --from=builder /usr/src/app/server /app/server
COPY --from=builder /usr/src/app/client /app/client

EXPOSE 12345

CMD ["/app/server"]
