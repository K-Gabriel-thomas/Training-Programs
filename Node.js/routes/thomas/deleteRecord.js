module.exports = async (req, res, db) => {
    const { tableName, id } = { ...req.query, ...req.body };
    if (!tableName || !id) return res.send("0");

    try {
        const [_, fields] = await db.query(`SELECT * FROM ?? LIMIT 1`, [tableName]);
        const pk = fields[0].name;

        const [result] = await db.execute(`DELETE FROM ?? WHERE ?? = ?`, [tableName, pk, id]);
        res.send(result.affectedRows > 0 ? "1" : "0");
    } catch (err) {
        res.send("0");
    }
};
