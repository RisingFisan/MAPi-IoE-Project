import mysql from 'mysql2/promise';
import dotenv from 'dotenv';

dotenv.config();

const pool = mysql.createPool({
  host: process.env.DB_HOST,
  port: Number(process.env.DB_PORT ?? 3306),
  user: process.env.DB_USER,
  password: process.env.DB_PASSWORD,
  database: process.env.DB_NAME,
  waitForConnections: true,
  connectionLimit: Number(process.env.DB_CONNECTION_LIMIT ?? 10),
  queueLimit: 0
});

const sensorsTableSql = `
  CREATE TABLE IF NOT EXISTS sensors (
    room_id INT NOT NULL,
    temp INT NOT NULL,
    humid INT NOT NULL,
    co INT NOT NULL,
    light INT NOT NULL,
    timestamp INT NOT NULL
  ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
`;

export async function ensureSensorsTable() {
  await pool.execute(sensorsTableSql);
}

export async function query(sql, params = []) {
  const [rows] = await pool.execute(sql, params);
  return rows;
}
