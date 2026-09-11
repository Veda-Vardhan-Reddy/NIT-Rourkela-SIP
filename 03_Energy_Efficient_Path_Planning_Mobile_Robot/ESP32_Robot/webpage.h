#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"====(
<!doctype html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP32 Robot + IMU Dashboard</title>

<script src="https://cdn.jsdelivr.net/npm/jspdf@2.5.1/dist/jspdf.umd.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/html2canvas@1.4.1/dist/html2canvas.min.js"></script>

<style>
:root{
  --bg:#0f172a;
  --card:#111827;
  --muted:#94a3b8;
  --text:#e5e7eb;
  --accent:#22c55e;
  --accent2:#38bdf8;
  --danger:#ef4444;
  --warn:#f59e0b
}

*{
  box-sizing:border-box
}

body{
  margin:0;
  font-family:Arial,sans-serif;
  background:linear-gradient(135deg,#0f172a,#111827);
  color:var(--text)
}

.wrap{
  max-width:1200px;
  margin:auto;
  padding:20px
}

.grid{
  display:grid;
  grid-template-columns:repeat(auto-fit,minmax(260px,1fr));
  gap:16px
}

.card{
  background:rgba(17,24,39,.92);
  border:1px solid #1f2937;
  border-radius:16px;
  padding:16px;
  box-shadow:0 10px 30px rgba(0,0,0,.25)
}

h1,h2{
  margin:0 0 12px
}

.muted{
  color:var(--muted);
  font-size:14px
}

.row{
  display:flex;
  gap:10px;
  flex-wrap:wrap;
  margin-top:10px
}

input,select,button{
  border-radius:12px;
  border:1px solid #334155;
  padding:12px;
  font-size:15px;
  background:#0b1220;
  color:var(--text)
}

input,select{
  width:100%;
  margin-bottom:8px
}

button{
  cursor:pointer;
  background:var(--accent);
  color:#03130a;
  border:none;
  font-weight:700
}

button.alt{
  background:var(--accent2);
  color:#04141b
}

button.stop{
  background:var(--danger);
  color:white
}

button.warn{
  background:var(--warn);
  color:#1b1300
}

.stat{
  font-size:28px;
  font-weight:700
}

.pill{
  display:inline-block;
  padding:6px 10px;
  border-radius:999px;
  background:#1e293b;
  color:#bfdbfe;
  font-size:13px
}

.two{
  display:grid;
  grid-template-columns:1fr 1fr;
  gap:10px
}

.three{
  display:grid;
  grid-template-columns:1fr 1fr 1fr;
  gap:10px
}

.manualGrid{
  display:grid;
  grid-template-columns:repeat(3,1fr);
  gap:10px
}

.manualGrid button{
  min-height:64px;
  font-size:24px
}

.smallStat{
  font-size:20px;
  font-weight:700
}

.logBox{
  max-height:260px;
  overflow:auto;
  background:#08101d;
  border-radius:12px;
  padding:10px;
  border:1px solid #22304a;
  font-family:monospace;
  font-size:13px;
  line-height:1.5;
  white-space:pre-wrap
}

.table{
  width:100%;
  border-collapse:collapse;
  font-size:13px
}

.table th,
.table td{
  padding:8px;
  border-bottom:1px solid #22304a;
  text-align:left
}

.table th{
  color:#93c5fd
}

.sensorBox{
  display:grid;
  grid-template-columns:1fr 1fr;
  gap:10px;
  margin-top:10px
}

.sensorTile{
  background:#0b1220;
  border:1px solid #22304a;
  border-radius:12px;
  padding:12px;
  text-align:center
}

.sensorState{
  font-size:22px;
  font-weight:700
}

@media(max-width:600px){
  .three{
    grid-template-columns:1fr 1fr
  }

  .two{
    grid-template-columns:1fr 1fr
  }

  .wrap{
    padding:10px
  }

  .table{
    display:block;
    overflow-x:auto;
    white-space:nowrap
  }
}
</style>
</head>

<body>

<div class="wrap" id="reportRoot">

<h1>ESP32 Robot + IMU Dashboard</h1>

<p class="muted">
Live robot, encoder, IMU, and IR line sensor data.
</p>

<div class="grid">

<!-- STATUS -->
<div class="card">

<h2>Status</h2>

<div class="pill" id="mode">idle</div>

<div class="row two">

<div>
<div class="muted">Encoder A</div>
<div class="stat" id="countA">0</div>
</div>

<div>
<div class="muted">Encoder B</div>
<div class="stat" id="countB">0</div>
</div>

</div>

<div class="row two">

<div>
<div class="muted">Meters A</div>
<div class="stat" id="metersA">0</div>
</div>

<div>
<div class="muted">Meters B</div>
<div class="stat" id="metersB">0</div>
</div>

</div>

<div class="row">

<button class="stop" onclick="apiCall('/stop')">
Stop
</button>

<button class="alt" onclick="apiCall('/resetEncoders')">
Reset Encoders
</button>

</div>

</div>


<!-- IR SENSORS -->
<div class="card">

<h2>IR line sensors</h2>

<div class="sensorBox">

<div class="sensorTile">
<div class="muted">Left IR</div>
<div class="sensorState" id="irLeft">0</div>
</div>

<div class="sensorTile">
<div class="muted">Right IR</div>
<div class="sensorState" id="irRight">0</div>
</div>

</div>

<p class="muted">
Usually 0 means line detected on many modules, but test your sensor once because some modules work the opposite way.
</p>

</div>


<!-- LINE FOLLOWER -->
<div class="card">

<h2>Line follower</h2>

<label>Line follow speed</label>

<input id="lineSpeed" value="130" type="number" min="0" max="255">

<div class="row">

<button onclick="startLineFollow()">
Start line follow
</button>

<button class="stop" onclick="stopLineFollow()">
Stop line follow
</button>

</div>

</div>


<!-- IMU -->
<div class="card">

<h2>IMU live</h2>

<div class="three">

<div>
<div class="muted">Ax m/s²</div>
<div class="smallStat" id="imuAxMs2">0</div>
</div>

<div>
<div class="muted">Ay m/s²</div>
<div class="smallStat" id="imuAyMs2">0</div>
</div>

<div>
<div class="muted">Az m/s²</div>
<div class="smallStat" id="imuAzMs2">0</div>
</div>

<div>
<div class="muted">Wx °/s</div>
<div class="smallStat" id="imuWx">0</div>
</div>

<div>
<div class="muted">Wy °/s</div>
<div class="smallStat" id="imuWy">0</div>
</div>

<div>
<div class="muted">Wz °/s</div>
<div class="smallStat" id="imuWz">0</div>
</div>

<div>
<div class="muted">Roll °</div>
<div class="smallStat" id="imuRoll">0</div>
</div>

<div>
<div class="muted">Pitch °</div>
<div class="smallStat" id="imuPitch">0</div>
</div>

<div>
<div class="muted">Yaw °</div>
<div class="smallStat" id="imuYaw">0</div>
</div>

</div>

</div>


<!-- ROBOT CONFIG -->
<div class="card">

<h2>Robot config</h2>

<label>Wheel diameter (meters)</label>
<input id="wheelDiameterM" value="0.085" type="number" step="0.001">

<label>Pulses per wheel revolution</label>
<input id="pulsesPerWheelRev" value="360" type="number">

<label>Wheel base (meters)</label>
<input id="wheelBaseM" value="0.16" type="number" step="0.001">

<label>Default speed (0-255)</label>
<input id="defaultSpeed" value="150" type="number" min="0" max="255">

<div class="row">

<button onclick="saveConfig()">
Save config
</button>

</div>

</div>


<!-- MANUAL DRIVE -->
<div class="card">

<h2>Manual drive</h2>

<label>Manual speed</label>

<input id="manualSpeed" value="150" type="number" min="0" max="255">

<div class="manualGrid">

<button onmousedown="manual('forwardLeft')"
        ontouchstart="manual('forwardLeft')">
↖
</button>

<button onmousedown="manual('forward')"
        ontouchstart="manual('forward')">
↑
</button>

<button onmousedown="manual('forwardRight')"
        ontouchstart="manual('forwardRight')">
↗
</button>

<button onmousedown="manual('left')"
        ontouchstart="manual('left')">
←
</button>

<button class="stop"
        onmousedown="manual('stop')"
        ontouchstart="manual('stop')">
■
</button>

<button onmousedown="manual('right')"
        ontouchstart="manual('right')">
→
</button>

<button onmousedown="manual('backwardLeft')"
        ontouchstart="manual('backwardLeft')">
↙
</button>

<button onmousedown="manual('backward')"
        ontouchstart="manual('backward')">
↓
</button>

<button onmousedown="manual('backwardRight')"
        ontouchstart="manual('backwardRight')">
↘
</button>

</div>

</div>


<!-- DISTANCE -->
<div class="card">

<h2>Distance move</h2>

<label>
Meters, positive forward, negative backward
</label>

<input id="meters" value="0.5" type="number" step="0.01">

<label>Speed</label>

<input id="moveSpeed"
       value="150"
       type="number"
       min="0"
       max="255">

<div class="row">

<button onclick="moveDistance()">
Run distance
</button>

</div>

</div>


<!-- TURN -->
<div class="card">

<h2>Turn</h2>

<label>
Degrees, positive right, negative left
</label>

<input id="degrees" value="90" type="number">

<label>Speed</label>

<input id="turnSpeed"
       value="150"
       type="number"
       min="0"
       max="255">

<div class="row">

<button onclick="turnRobot()">
Run turn
</button>

</div>

</div>


<!-- SHAPES -->
<div class="card">

<h2>Shapes</h2>

<label>Shape</label>

<select id="shapeName">

<option value="square">Square</option>
<option value="triangle">Triangle</option>
<option value="circle">Circle</option>

</select>

<label>
Size, square/triangle = side meters, circle = radius meters
</label>

<input id="shapeSize"
       value="0.4"
       type="number"
       step="0.01">

<label>Speed</label>

<input id="shapeSpeed"
       value="140"
       type="number"
       min="0"
       max="255">

<div class="row">

<button onclick="runShape()">
Run shape
</button>

</div>

</div>


<!-- RECORDER -->
<div class="card">

<h2>Recorder</h2>

<label>Sample period (ms)</label>

<input id="recordPeriod"
       value="200"
       type="number"
       min="50">

<div class="row">

<button class="warn" onclick="startRecording()">
Start record
</button>

<button class="alt" onclick="stopRecording()">
Stop record
</button>

<button onclick="downloadCsv()">
Download CSV
</button>

<button onclick="downloadPdf()">
Save PDF
</button>

</div>

<p class="muted">
Samples recorded:
<span id="sampleCount">0</span>
</p>

</div>


<!-- RECENT SAMPLES -->
<div class="card" style="grid-column:1/-1">

<h2>Recent recorded samples</h2>

<div class="logBox" id="logBox">
No samples yet.
</div>

</div>


<!-- REPORT -->
<div class="card" style="grid-column:1/-1">

<h2>Report preview</h2>

<div style="overflow-x:auto">

<table class="table" id="reportTable">

<thead>

<tr>
<th>Time</th>
<th>Mode</th>
<th>Enc A</th>
<th>Enc B</th>
<th>IR L</th>
<th>IR R</th>
<th>mA</th>
<th>mB</th>
<th>Ax</th>
<th>Ay</th>
<th>Az</th>
<th>Roll</th>
<th>Pitch</th>
<th>Yaw</th>
</tr>

</thead>

<tbody id="reportBody"></tbody>

</table>

</div>

</div>

</div>
</div>


<script>

var samples = [];
var latestStatus = null;
var recordTimer = null;


/* API CALL */

function apiCall(url) {

  return fetch(url)
    .then(function(response) {

      if (!response.ok) {
        throw new Error("HTTP error " + response.status);
      }

      return response.text();

    })
    .catch(function(error) {

      console.error("API error:", error);

    });

}


/* REFRESH STATUS */

function refresh() {

  fetch('/status')

    .then(function(response) {

      if (!response.ok) {
        throw new Error("Status request failed");
      }

      return response.json();

    })

    .then(function(s) {

      latestStatus = s;

      document.getElementById('mode').textContent =
        s.mode !== undefined ? s.mode : 'unknown';

      document.getElementById('countA').textContent =
        s.countA !== undefined ? s.countA : 0;

      document.getElementById('countB').textContent =
        s.countB !== undefined ? s.countB : 0;

      document.getElementById('metersA').textContent =
        Number(s.metersA || 0).toFixed(3);

      document.getElementById('metersB').textContent =
        Number(s.metersB || 0).toFixed(3);


      document.getElementById('imuAxMs2').textContent =
        Number(s.imuAxMs2 || 0).toFixed(3);

      document.getElementById('imuAyMs2').textContent =
        Number(s.imuAyMs2 || 0).toFixed(3);

      document.getElementById('imuAzMs2').textContent =
        Number(s.imuAzMs2 || 0).toFixed(3);


      document.getElementById('imuWx').textContent =
        Number(s.imuWx || 0).toFixed(3);

      document.getElementById('imuWy').textContent =
        Number(s.imuWy || 0).toFixed(3);

      document.getElementById('imuWz').textContent =
        Number(s.imuWz || 0).toFixed(3);


      document.getElementById('imuRoll').textContent =
        Number(s.imuRoll || 0).toFixed(2);

      document.getElementById('imuPitch').textContent =
        Number(s.imuPitch || 0).toFixed(2);

      document.getElementById('imuYaw').textContent =
        Number(s.imuYaw || 0).toFixed(2);


      document.getElementById('irLeft').textContent =
        s.irLeft !== undefined ? s.irLeft : 0;

      document.getElementById('irRight').textContent =
        s.irRight !== undefined ? s.irRight : 0;

    })

    .catch(function(error) {

      console.error("Refresh error:", error);

    });

}


/* SAVE CONFIG */

function saveConfig() {

  var q = new URLSearchParams({

    wheelDiameterM:
      document.getElementById('wheelDiameterM').value,

    pulsesPerWheelRev:
      document.getElementById('pulsesPerWheelRev').value,

    wheelBaseM:
      document.getElementById('wheelBaseM').value,

    defaultSpeed:
      document.getElementById('defaultSpeed').value

  });

  apiCall('/config?' + q.toString())

    .then(function(text) {

      if (text) {
        alert(text);
      }

    });

}


/* MANUAL DRIVE */

function manual(cmd) {

  var q = new URLSearchParams({

    cmd: cmd,

    speed:
      document.getElementById('manualSpeed').value

  });

  apiCall('/manual?' + q.toString());

}


/* DISTANCE MOVE */

function moveDistance() {

  var q = new URLSearchParams({

    meters:
      document.getElementById('meters').value,

    speed:
      document.getElementById('moveSpeed').value

  });

  apiCall('/move?' + q.toString());

}


/* TURN */

function turnRobot() {

  var q = new URLSearchParams({

    degrees:
      document.getElementById('degrees').value,

    speed:
      document.getElementById('turnSpeed').value

  });

  apiCall('/turn?' + q.toString());

}


/* SHAPE */

function runShape() {

  var q = new URLSearchParams({

    name:
      document.getElementById('shapeName').value,

    size:
      document.getElementById('shapeSize').value,

    speed:
      document.getElementById('shapeSpeed').value

  });

  apiCall('/shape?' + q.toString());

}


/* LINE FOLLOWER */

function startLineFollow() {

  var q = new URLSearchParams({

    speed:
      document.getElementById('lineSpeed').value

  });

  apiCall('/lineFollowStart?' + q.toString());

}


function stopLineFollow() {

  apiCall('/lineFollowStop');

}


/* ADD SAMPLE */

function addSample() {

  if (!latestStatus) {
    return;
  }

  var stamp = new Date().toLocaleString();

  var s = Object.assign(
    {time: stamp},
    latestStatus
  );

  samples.push(s);

  document.getElementById('sampleCount').textContent =
    samples.length;


  var line =
    stamp +
    " | " + (s.mode || "") +
    " | A " + (s.countA || 0) +
    " | B " + (s.countB || 0) +
    " | IRL " + (s.irLeft || 0) +
    " | IRR " + (s.irRight || 0) +
    " | Ax " + Number(s.imuAxMs2 || 0).toFixed(3) +
    " | Ay " + Number(s.imuAyMs2 || 0).toFixed(3) +
    " | Az " + Number(s.imuAzMs2 || 0).toFixed(3) +
    " | Roll " + Number(s.imuRoll || 0).toFixed(2) +
    " | Pitch " + Number(s.imuPitch || 0).toFixed(2) +
    " | Yaw " + Number(s.imuYaw || 0).toFixed(2);


  var logBox =
    document.getElementById('logBox');


  if (logBox.textContent === 'No samples yet.') {

    logBox.textContent = '';

  }


  logBox.textContent += line + "\n";

  logBox.scrollTop = logBox.scrollHeight;


  /* TABLE ROW */

  var row = document.createElement('tr');

  row.innerHTML =
    "<td>" + s.time + "</td>" +
    "<td>" + (s.mode || "") + "</td>" +
    "<td>" + (s.countA || 0) + "</td>" +
    "<td>" + (s.countB || 0) + "</td>" +
    "<td>" + (s.irLeft || 0) + "</td>" +
    "<td>" + (s.irRight || 0) + "</td>" +
    "<td>" + Number(s.metersA || 0).toFixed(3) + "</td>" +
    "<td>" + Number(s.metersB || 0).toFixed(3) + "</td>" +
    "<td>" + Number(s.imuAxMs2 || 0).toFixed(3) + "</td>" +
    "<td>" + Number(s.imuAyMs2 || 0).toFixed(3) + "</td>" +
    "<td>" + Number(s.imuAzMs2 || 0).toFixed(3) + "</td>" +
    "<td>" + Number(s.imuRoll || 0).toFixed(2) + "</td>" +
    "<td>" + Number(s.imuPitch || 0).toFixed(2) + "</td>" +
    "<td>" + Number(s.imuYaw || 0).toFixed(2) + "</td>";


  document
    .getElementById('reportBody')
    .appendChild(row);

}


/* START RECORDING */

function startRecording() {

  stopRecording();

  var period =
    Math.max(
      50,
      parseInt(
        document.getElementById('recordPeriod').value || '200',
        10
      )
    );

  recordTimer =
    setInterval(addSample, period);

  addSample();

}


/* STOP RECORDING */

function stopRecording() {

  if (recordTimer) {

    clearInterval(recordTimer);

    recordTimer = null;

  }

}


/* DOWNLOAD CSV */

function downloadCsv() {

  if (samples.length === 0) {

    alert('No samples recorded');

    return;

  }


  var headers = [

    'time',
    'mode',
    'countA',
    'countB',
    'irLeft',
    'irRight',
    'metersA',
    'metersB',
    'imuAxMs2',
    'imuAyMs2',
    'imuAzMs2',
    'imuWx',
    'imuWy',
    'imuWz',
    'imuRoll',
    'imuPitch',
    'imuYaw'

  ];


  var csv =
    headers.join(',') + "\n";


  for (var i = 0; i < samples.length; i++) {

    var row = [];

    for (var j = 0; j < headers.length; j++) {

      var key = headers[j];

      var value =
        samples[i][key];

      if (value === undefined || value === null) {
        value = "";
      }

      row.push(
        JSON.stringify(String(value))
      );

    }

    csv += row.join(',') + "\n";

  }


  var blob =
    new Blob(
      [csv],
      {type:'text/csv;charset=utf-8;'}
    );


  var a =
    document.createElement('a');

  var url =
    URL.createObjectURL(blob);

  a.href = url;

  a.download =
    'robot_imu_log.csv';

  document.body.appendChild(a);

  a.click();

  document.body.removeChild(a);

  URL.revokeObjectURL(url);

}


/* DOWNLOAD PDF */

function downloadPdf() {

  if (samples.length === 0) {

    alert('No samples recorded');

    return;

  }


  if (
    typeof window.jspdf === 'undefined' ||
    typeof html2canvas === 'undefined'
  ) {

    alert(
      'PDF libraries are not loaded. Check your internet connection.'
    );

    return;

  }


  var jsPDFRef =
    window.jspdf.jsPDF;


  html2canvas(

    document.querySelector('#reportRoot'),

    {
      scale:2,
      backgroundColor:'#0f172a',
      useCORS:true
    }

  )

  .then(function(canvas) {

    var img =
      canvas.toDataURL('image/png');


    var pdf =
      new jsPDFRef(
        'p',
        'mm',
        'a4'
      );


    var pageWidth = 190;

    var pageHeight = 277;

    var imgWidth = pageWidth;

    var imgHeight =
      canvas.height *
      imgWidth /
      canvas.width;


    var heightLeft =
      imgHeight;


    var position = 10;


    pdf.addImage(
      img,
      'PNG',
      10,
      position,
      imgWidth,
      imgHeight
    );


    heightLeft -= pageHeight;


    while (heightLeft > 0) {

      position =
        heightLeft - imgHeight + 10;

      pdf.addPage();

      pdf.addImage(
        img,
        'PNG',
        10,
        position,
        imgWidth,
        imgHeight
      );

      heightLeft -= pageHeight;

    }


    pdf.save(
      'robot_imu_report.pdf'
    );

  })

  .catch(function(error) {

    console.error(
      'PDF generation error:',
      error
    );

    alert(
      'Failed to generate PDF.'
    );

  });

}


/* REFRESH EVERY 250 ms */

setInterval(
  refresh,
  250
);

refresh();

</script>

</body>
</html>
)====";

#endif