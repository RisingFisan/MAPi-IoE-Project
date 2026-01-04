export type SensorReading = {
  room_id: number;
  temp: number;
  humid: number;
  co: number;
  light: number;
  people_count: number;
  timestamp: number;
};

export interface SensorResponse {
  roomId: string;
  readings: SensorReading[];
}

export type SensorMetric = 'temp' | 'humid' | 'co' | 'light' | 'people_count';

export const SENSOR_LABELS: Record<SensorMetric, string> = {
  temp: 'Temperature (°C)',
  humid: 'Humidity (%)',
  co: 'CO (%)',
  light: 'Light (%)',
  people_count: 'People Count',
};
