// This test could be run on browser and qunit-cli.
// See https://github.com/devongovett/qunit-cli
// IMPORTANT: Some test could be run on browser only!!
if (typeof QUnit == "undefined") QUnit = require("qunit-cli");
if (typeof require != "undefined") DisplayLiFi = require("../lib/displaylifi.js");

function getPixelOnTopLeft(context) {
  var data = context.getImageData(1, 1, 1, 1).data;
  return {"red": data[0], "green": data[1], "blue": data[2]};
}

var gMessageShown = false;
var gCanvas = (typeof jQuery != "undefined") ? $("canvas")[0] : null;

function checkCLI() {
  if (typeof require != "undefined") {
    if (!gMessageShown) {
      console.log("Warning: Some tests cannot run on CLI.");
      gMessageShown = true;
    }
    assert.ok(true);
    return true;
  }
  return false;
}

QUnit.test( "fillColor high", function(assert) {
  if (checkCLI()) return;
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.fillColor(1, gCanvas);
  color = getPixelOnTopLeft(gCanvas.getContext("2d"));
  assert.equal(color.red, 255);
  assert.equal(color.green, 255);
  assert.equal(color.blue, 255);
});

QUnit.test( "fillColor low", function(assert) {
  if (checkCLI()) return;
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.fillColor(0, gCanvas);
  color = getPixelOnTopLeft($("canvas")[0].getContext("2d"));
  assert.equal(color.red, 0);
  assert.equal(color.green, 0);
  assert.equal(color.blue, 0);
});

QUnit.test( "pushOneBIt 0", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(0);
  var queue = lifi.getQueue();
  assert.equal(2, queue.length);
  assert.equal(1, queue[0].signal);
  assert.equal(50, queue[0].duration);
  assert.equal(0, queue[1].signal);
  assert.equal(100, queue[1].duration);
});

QUnit.test( "pushOneBit 1", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(1);
  var queue = lifi.getQueue();
  assert.equal(2, queue.length);
  assert.equal(1, queue[0].signal);
  assert.equal(50, queue[0].duration);
  assert.equal(0, queue[1].signal);
  assert.equal(300, queue[1].duration);
});

QUnit.test( "pushOneBit 110", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(1);
  lifi.pushOneBit(1);
  lifi.pushOneBit(0);
  var queue = lifi.getQueue();
  assert.equal(2 * 3, queue.length);
  assert.equal(1, queue[0].signal);
  assert.equal(50, queue[0].duration);
  assert.equal(0, queue[1].signal);
  assert.equal(300, queue[1].duration);
  assert.equal(1, queue[2].signal);
  assert.equal(50, queue[2].duration);
  assert.equal(0, queue[3].signal);
  assert.equal(300, queue[3].duration);
  assert.equal(1, queue[4].signal);
  assert.equal(50, queue[4].duration);
  assert.equal(0, queue[5].signal);
  assert.equal(100, queue[5].duration);
});

QUnit.test( "pushChar", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushChar("a"); // ascii:97(0b1100001)
  var queue = lifi.getQueue();
  assert.equal(2 * 8, queue.length);
  var i = 0;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);
});

QUnit.test( "pushString", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushString("ab"); // ascii:97(0b01100001) ascii:98(0b01100010)
  var queue = lifi.getQueue();
  assert.equal(2 * 8 * 2, queue.length);
  var i = 0;
  // a
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 1
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 1
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 1
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  // b
  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 1
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 1
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 1
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal); // 0
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);
});

QUnit.test( "computeMSecFromStart 10", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(1);
  lifi.pushOneBit(0);
  lifi.computeMSecFromStart();

  var queue = lifi.getQueue();
  assert.equal(2 * 2, queue.length);
  var i = 0;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i].msecFromStart);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);
  assert.equal(50, queue[i + 1].msecFromStart);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(350, queue[i].msecFromStart);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);
  assert.equal(400, queue[i + 1].msecFromStart);
});

QUnit.test( "start", function(assert) {
  if (checkCLI()) return;
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushString("ab"); // ascii:97(0b01100001) ascii:98(0b01100010)
  lifi.start($("canvas")[0]);
  var queue = lifi.getQueue();
  assert.equal(2 + 2 * 8 * 2 + 2, queue.length);

  // leader code
  var i = 0;
  assert.equal(1, queue[i].signal);
  assert.equal(600, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(600, queue[i + 1].duration);

  // a
  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  // b
  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(300, queue[i + 1].duration);

  i += 2;
  assert.equal(1, queue[i].signal);
  assert.equal(50, queue[i].duration);
  assert.equal(0, queue[i + 1].signal);
  assert.equal(100, queue[i + 1].duration);
});

QUnit.test("insertLeaderCodeAtFirst", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(1);
  lifi.insertLeaderCodeAtFirst();
  var queue = lifi.getQueue();
  assert.equal(1, queue[0].signal);
  assert.equal(600, queue[0].duration);
  assert.equal(0, queue[1].signal);
  assert.equal(600, queue[1].duration);
  assert.equal(1, queue[2].signal);
  assert.equal(50, queue[2].duration);
  assert.equal(0, queue[3].signal);
  assert.equal(300, queue[3].duration);
});

QUnit.test("existsLeaderCode false", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(1);
  assert.equal(false, lifi.existsLeaderCode());
});

QUnit.test("existsLeaderCode true", function(assert) {
  var lifi = new DisplayLiFi({leaderCodeCountAtFirst: 1, leaderCodeCountAtEnd: 1});
  lifi.pushOneBit(1);
  lifi.insertLeaderCodeAtFirst();
  assert.equal(true, lifi.existsLeaderCode());
});
