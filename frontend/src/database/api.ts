
const API_ADDR = '192.168.4.23:3000';

export const fetchRoomData = async (roomId: number) => {
  let data = defaultData(roomId);
  try {
    const response = await fetch(`http://${API_ADDR}/rooms/${roomId}/sensor`);
    data = await response.json();
  } catch {
    console.error("API down — using test response");
  }
  return data;
}

export const listRooms = async () => {
  return JSON.parse(`[
    {
      "roomId": "1",
      "roomName": "Room 1"
    },
    {
      "roomId": "2",
      "roomName": "Room 2"
    },
    {
      "roomId": "3",
      "roomName": "Room 3"
    },
    {
      "roomId": "4",
      "roomName": "Room 4"
    },
    {
      "roomId": "5",
      "roomName": "Room 5"
    },
    {
      "roomId": "6",
      "roomName": "Room 6"
    }
  ]`);
}

const defaultData = (roomId: number) => {
  return JSON.parse(`
{
  "roomId": "${roomId}",
  "readings": [
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 11,
      "timestamp": 2083187
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 11,
      "timestamp": 2078166
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 11,
      "timestamp": 2073128
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 12,
      "timestamp": 2068103
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 10,
      "timestamp": 2063074
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 10,
      "timestamp": 2058024
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 11,
      "timestamp": 2053008
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 10,
      "timestamp": 2047985
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 11,
      "timestamp": 2042968
    },
    {
      "room_id": ${roomId},
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 10,
      "timestamp": 2037964
    }
  ]
}
            `)
}
/*

{
  "roomId": "1",
  "readings": [
    {
      "room_id": 1,
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 11,
      "timestamp": 2083187
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 11,
      "timestamp": 2078166
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 11,
      "timestamp": 2073128
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 12,
      "timestamp": 2068103
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 10,
      "timestamp": 2063074
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 66,
      "co": 17,
      "light": 10,
      "timestamp": 2058024
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 11,
      "timestamp": 2053008
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 10,
      "timestamp": 2047985
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 11,
      "timestamp": 2042968
    },
    {
      "room_id": 1,
      "temp": 23,
      "humid": 67,
      "co": 17,
      "light": 10,
      "timestamp": 2037964
    }
  ]
}

