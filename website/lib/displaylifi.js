/**
 * Created by ikeyasu
 */

/**
 * Protocol:
 * It based on PPM, Pulse Position Modulation.
 * http://en.wikipedia.org/wiki/Pulse-position_modulation
 **/

(function () {
  var SENDER_COLOR_0 = "rgb(0, 0, 0)";
  var SENDER_COLOR_1 = "rgb(255,255,255)";
  var DURATION_0 = 100; //msec
  var DURATION_1 = 300; //msec
  var HIGH_DURATION = 50; //msec
  var LEADER_CODE_HIGH_DURATION = 1000; //msec
  var LEADER_CODE_LOW_DURATION = 1000; //msec

  function DisplayLiFi(options) {

    var queue_ = [];
    var leaderCodeCountAtFirst_ = 5;
    var leaderCodeCountAtEnd_ = 2;
    var queueComputed_ = false;
    var self = this;

    if (typeof options != "undefined") {
      leaderCodeCountAtFirst_ = options["leaderCodeCountAtFirst"] ? options["leaderCodeCountAtFirst"] : leaderCodeCountAtFirst_;
      leaderCodeCountAtEnd_ = options["leaderCodeCountAtEnd"] ? options["leaderCodeCountAtEnd"] : leaderCodeCountAtEnd_;
    }

    var r = "equestAnimationFrame";
    this['r' + r] = this['r' + r] || this['webkitR' + r] || this['mozR' + r] || this['msR' + r] || this['oR' + r] ||
      function (c) {
        setTimeout(c, 1000 / 100);
      };

    function getNowMSec() {
      if (typeof window.performance != "undefined" && typeof window.performance.now != "undefined") {
        return window.performance.now();
      }
      return Date.now();
    }

    this.getQueue = function () {
      return queue_;
    };

    this.getProgress = function () {
      if (queue_.length === 0) return 0;
      return currentIndexOfQueue_ / queue_.length;
    };

    this.isDone = function () {
      if (queue_.length === 0) return false;
      return currentIndexOfQueue_ === queue_.length;
    };

    this.fillColor = function (bit, canvas) {
      var context = canvas.getContext("2d");
      context.fillStyle = bit ? SENDER_COLOR_1 : SENDER_COLOR_0;
      context.fillRect(0, 0, $(canvas).width(), $(canvas).height());
    };

    this.pushOneBit = function (bit) {
      if (bit > 1/*0b1*/) {
        throw "`sendOneBit()` accepts one bit number only.";
      }
      queue_.push({signal: 1, duration: HIGH_DURATION});
      queue_.push({signal: 0, duration: bit == 0 ? DURATION_0 - HIGH_DURATION: DURATION_1 - HIGH_DURATION});
      queueComputed_ = false;
    };

    this.insertLeaderCodeAtFirst = function () {
      for (var i = 0; i < leaderCodeCountAtFirst_; i++) {
        queue_.splice(0, 0, {signal: 1, duration: LEADER_CODE_HIGH_DURATION});
        queue_.splice(1, 0, {signal: 0, duration: LEADER_CODE_LOW_DURATION});
      }
      queueComputed_ = false;
    };

    this.insertLeaderCodeAtEnd = function () {
      for (var i = 0; i < leaderCodeCountAtEnd_; i++) {
        queue_.push({signal: 1, duration: LEADER_CODE_HIGH_DURATION});
        queue_.push({signal: 0, duration: LEADER_CODE_LOW_DURATION});
      }
      queueComputed_ = false;
    };

    this.pushChar = function (character) {
      var ascii = character.charCodeAt(0);
      for (var j = 7; j >= 0; j--) {
        this.pushOneBit((ascii >> j) & 0x1);
      }
    };

    this.pushString = function (string) {
      for (var j = 0; j < string.length; j++) {
        this.pushChar(string[j]);
      }
    };

    this.computeMSecFromStart = function () {
      var time = 0; // msec
      for (var i = 0; i < queue_.length; i++) {
        queue_[i].msecFromStart = time;
        time += queue_[i].duration;
      }
      queueComputed_ = true;
    };

    this.existsLeaderCode = function () {
      if (queue_[0].signal != 1 || queue_[0].duration != LEADER_CODE_HIGH_DURATION) {
        return false;
      }
      if (queue_[1].signal != 0 || queue_[1].duration != LEADER_CODE_LOW_DURATION) {
        return false;
      }
      return true;
    };

    var startMSec_ = 0;
    var currentIndexOfQueue_ = 0;
    var canvas_ = null;
    function loop() {
      if (queue_[currentIndexOfQueue_].msecFromStart <= (getNowMSec() - startMSec_)) {
        self.fillColor(queue_[currentIndexOfQueue_].signal, canvas_);
        currentIndexOfQueue_++;
        if (currentIndexOfQueue_ >= queue_.length) {
          return;
        }
      }
      requestAnimationFrame(loop);
    };

    this.start = function (canvas) {
      if (!this.existsLeaderCode()) {
        this.insertLeaderCodeAtFirst();
        this.insertLeaderCodeAtEnd();
      }
      if (!queueComputed_) {
        this.computeMSecFromStart();
      }
      startMSec_ = getNowMSec();
      currentIndexOfQueue_ = 0;
      canvas_ = canvas;
      requestAnimationFrame(loop);
    };
  }

  if (typeof exports == "undefined") {
    this["DisplayLiFi"] = DisplayLiFi;
  } else {
    module.exports = DisplayLiFi;
  }

})();
