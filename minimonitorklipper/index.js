const fetch = (...args) =>
  import("node-fetch").then(({ default: fetch }) => fetch(...args));
const express = require("express");
const app = express();
app.use(express.json());
const port = 3000;

app.get("/temps", async (req, res) => {
  const request = await fetch(
    "http://localhost/server/temperature_store?include_monitors=false"
  );
  const body = await request.json();
  //console.log(body);
  let bedLen = body.result.heater_bed.temperatures.length;
  let bedTargetsLen = body.result.heater_bed.targets.length;
  let extruderLen = body.result.extruder.temperatures.length;
  let extruderTargetLen = body.result.extruder.targets.length;

  res.json({
    bed: {
      temp: body.result.heater_bed.temperatures[bedLen - 1],
      target: body.result.heater_bed.targets[bedTargetsLen - 1],
    },
    extruder: {
      temp: body.result.extruder.temperatures[extruderLen - 1],
      target: body.result.extruder.targets[extruderTargetLen - 1],
    },
  });
});

app.listen(port, ()=>console.log("Running on port " + port));
