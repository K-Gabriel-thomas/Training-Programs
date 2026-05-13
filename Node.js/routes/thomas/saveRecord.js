module.exports = async (req, res, db) => {
    const data = { ...req.query, ...req.body };
    const { tableName } = data;
    if (!tableName) return res.send("0");

    try {
        const [_, fields] = await db.query(`SELECT * FROM ?? LIMIT 1`, [tableName]);
        const cols = fields.map(f => f.name);
        
        const values = cols.map(c => data[c] || "");
        const updateClause = cols.slice(1).map(c => `\`${c}\`=VALUES(\`${c}\`)`).join(',');

        const sql = `
            INSERT INTO ?? (\`${cols.join('`,`')}\`) 
            VALUES (${cols.map(() => '?').join(',')}) 
            ON DUPLICATE KEY UPDATE ${updateClause}
        `;

        await db.execute(sql, [tableName, ...values]);
        res.send("1");
    } catch (err) {
        console.error(err);
        res.send("0");
    }
};
