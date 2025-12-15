# mapi-api

API for room sensor data from a MySQL table named `mapi` containing the columns `room_id`, `temp`, `humid`, `co`, `light`, and `timestamp`.

## Setup

1. Install dependencies:

```bash
npm install
```

2. Copy the environment template and fill in your database credentials. Keep `DB_HOST=mysql` when running under Docker Compose; switch it to `localhost` (or another host) when running the API directly on your machine.

```bash
cp .env.example .env
```

3. Start the API locally:

```bash
npm start
```

The server listens on `PORT` (default `3000`).

## Docker Compose

The repo includes a `Dockerfile` and `docker-compose.yml` that start both the API and a MySQL 8 instance with matching credentials.

```bash
docker compose up --build
```

This command builds the API image, provisions MySQL with the schema in the `mapi` database, and exposes the API on `http://localhost:${PORT:-3000}`. Data is persisted in the `mysql_data` named volume. Stop everything with `docker compose down` (add `-v` to drop the database volume).

## Endpoint

- `GET /rooms/:roomId/sensor`
  - Returns every reading for the given `roomId`, ordered by `timestamp` descending.
  - Successful response shape:

```json
{
  "roomId": "101",
  "readings": [
    {
      "room_id": "101",
      "temp": 22.5,
      "humid": 41.2,
      "co": 0.3,
      "light": 350,
      "timestamp": "2025-11-24T00:00:00.000Z"
    }
  ]
}
```

404 is returned if the room has no readings; 400 if `roomId` is missing; 500 on unexpected errors.
