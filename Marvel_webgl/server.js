
let express = require('express');

let app = express();

app.use(express.static('public'));

app.listen(3000);