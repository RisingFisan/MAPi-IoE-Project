import mysql from 'mysql2/promise';

// --- Database Connection Configuration ---
const dbConfig = {
    host: '192.168.4.23', // Your remote server's IP address
    user: 'mapi',
    password: 'mapi',
    database: 'mapi'
};

async function connectAndQuery() {
    let connection: mysql.Connection | null = null;
    try {
        // Create the connection
        connection = await mysql.createConnection(dbConfig);
        console.log("✅ Connected to external MySQL server!");

        // Execute a simple query
        const [rows, fields] = await connection.execute('SELECT * FROM sensors');
        
        console.log('Query Result:', rows, fields);

    } catch (error) {
        console.error("❌ Database Error:", error);
    } finally {
        // Close the connection
        if (connection) {
            await connection.end();
            console.log("Connection closed.");
        }
    }
}

connectAndQuery();