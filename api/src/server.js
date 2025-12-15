import express from 'express';
import dotenv from 'dotenv';
import { query, ensureSensorsTable } from './db.js';

dotenv.config();

const app = express();
const PORT = Number(process.env.PORT ?? 3000);

app.get('/rooms/:roomId/sensor', async (req, res) => {
  const { roomId } = req.params;

  if (!roomId) {
    return res.status(400).json({ error: 'roomId is required' });
  }

  try {
    const sql = `
      SELECT room_id, temp, humid, co, light, timestamp
      FROM sensors
      WHERE room_id = ?
      ORDER BY timestamp DESC
      LIMIT 10
    `;

    const rows = await query(sql, [roomId]);

    if (rows.length === 0) {
      return res.status(404).json({ error: 'No sensor readings for that room_id' });
    }

    res.json({ roomId, readings: rows });
  } catch (error) {
    console.error('Failed to fetch sensor data', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

function startServer() {
  return ensureSensorsTable().then(() => {
    app.listen(PORT, () => {
      console.log(`Server listening on port ${PORT}`);
    });
  });
}

startServer().catch((error) => {
  console.error('Failed to start server', error);
  process.exit(1);
});
