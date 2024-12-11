FROM alpine:3.20 AS builder

RUN apk add --update cmake make gcc g++ libuv-dev rocksdb-dev
WORKDIR /fdb
COPY . .

# Building from the source
RUN mkdir build && cd build && \
  cmake -DCMAKE_BUILD_TYPE=Release .. && \
  cmake --build . --target fdb


FROM alpine:3.20 AS main
RUN apk add --update --no-cache libuv rocksdb
WORKDIR /app
COPY --from=builder /fdb/build/fdb .
COPY --from=builder /fdb/build/*.so .
CMD [ "./fdb" ]