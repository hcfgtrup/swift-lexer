FROM gcc:13 AS builder
WORKDIR /app
COPY src/ ./src/
RUN g++ -std=c++17 -O2 -Wall -Wextra -Isrc -static-libgcc -static-libstdc++ -o lexer src/main.cpp

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /app/lexer /usr/local/bin/lexer
ENTRYPOINT ["/usr/local/bin/lexer"]