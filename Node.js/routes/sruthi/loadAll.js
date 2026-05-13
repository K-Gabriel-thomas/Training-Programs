module.exports = async (req, res, db) => {
    const { tableName } = req.query;
    if (!tableName) return res.status(400).json({ error: "Missing Table" });

    try {
        const [rows, fields] = await db.query(`SELECT * FROM ??`, [tableName]);
        res.json({
            cols: fields.map(f => f.name),
            data: rows.map(r => Object.values(r))
        });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};
