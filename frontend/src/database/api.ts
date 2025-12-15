export const fetchData = async () => {
        const response = await fetch('http://192.168.4.23:3000/rooms/1/sensor');
        return response.json()
}

