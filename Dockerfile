# Wykorzystujemy bazę Ubuntu
FROM ubuntu:latest

# Unikanie pytań o strefę czasową podczas instalacji
ENV DEBIAN_FRONTEND=noninteractive

# Aktualizacja i instalacja narzędzi C++, make, cmake i gtest
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    cmake \
    make \
    libgtest-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Kompilacja biblioteki GTest (w Ubuntu źródła są w /usr/src/gtest)
RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

# Ustawienie katalogu roboczego
WORKDIR /app

CMD ["/bin/bash"]
