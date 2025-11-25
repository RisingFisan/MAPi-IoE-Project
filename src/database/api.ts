export const fetchData = async () => {
    try {
        const response = await fetch('http://192.168.4.23:3000/rooms/1/sensor');
        return response.json()
    } catch {
        return JSON.parse(`
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
            `)
    }
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

*/