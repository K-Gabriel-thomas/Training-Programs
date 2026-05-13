const express = require('express');
const mysql = require('mysql2/promise');
const cors = require('cors');
const fs = require('fs');
const path = require('path');

const app = express();
const port = 1323;

app.use(cors(), express.json(), express.urlencoded({ extended: true }));

const db = mysql.createPool({
    host: 'localhost',
    user: 'thomas1',
    password: 'Thomas1@##$$$',
    database: 'dbThomas1',
    waitForConnections: true,
    connectionLimit: 10
});

const routesDir = path.join(__dirname, 'routes');

function boot() {
    if (!fs.existsSync(routesDir)) fs.mkdirSync(routesDir);

    fs.readdirSync(routesDir).forEach(user => {
        const userPath = path.join(routesDir, user);
        if (!fs.statSync(userPath).isDirectory()) return;

        fs.readdirSync(userPath).forEach(file => {
            const filePath = path.join(userPath, file);
            const name = file.split('.')[0];
            const url = `/${user}/${name}`;

            // Handle Logic Files (.js)
            if (file.endsWith('.js')) {
                app.all(url, async (req, res) => {
                    try {
                        delete require.cache[require.resolve(filePath)];
                        await require(filePath)(req, res, db);
                    } catch (err) {
                        console.error(`Runtime Error [${url}]:`, err);
                        res.status(500).send("0");
                    }
                });
            } 
            // Handle Webpages (.html)
            else if (file.endsWith('.html')) {
                app.get(url, (req, res) => res.sendFile(filePath));
            }
        });
    });
}

boot();
app.listen(port, () => console.log(`🚀 Server on http://localhost:${port}`));
