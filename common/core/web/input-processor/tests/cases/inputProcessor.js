var assert = require('chai').assert;
var fs = require("fs");
var vm = require("vm");

let InputProcessor = require('../../dist');

// Required initialization setup.
global.com = InputProcessor.com; // exports all keyboard-processor namespacing.
global.keyman = {}; // So that keyboard-based checks against the global `keyman` succeed.
                    // 10.0+ dependent keyboards, like khmer_angkor, will otherwise fail to load.

// Initialize supplementary plane string extensions
String.kmwEnableSupplementaryPlane(false);    

// Test the KeyboardProcessor interface.
describe('InputProcessor', function() {
  describe('[[constructor]]', function () {
    it('should initialize without errors', function () {
      let core = new InputProcessor();
      assert.isNotNull(core);
    });

    it('has expected default values after initialization', function () {
      let core = new InputProcessor();

      assert.isOk(core.keyboardProcessor);
      assert.isOk(core.languageProcessor);
      assert.isOk(core.keyboardInterface);
      assert.isUndefined(core.activeKeyboard); // No keyboard should be loaded yet.
      assert.isUndefined(core.activeModel);    // Same for the model.

      // These checks are lifted from the keyboard-processor init checks found in
      // common/core/web/keyboard-processor/tests/cases/basic-init.js.
      assert.equal('us', core.keyboardProcessor.baseLayout, 'KeyboardProcessor has unexpected base layout')
      assert.isNotNull(global.KeymanWeb, 'KeymanWeb global was not automatically installed');
      assert.equal('default', core.keyboardProcessor.layerId, 'Default layer is not set to "default"');
      assert.isUndefined(core.keyboardProcessor.activeKeyboard, 'Initialized with already-active keyboard');

      // Lifted from languageProcessor.js - the core should not be changing these with its init.
      assert.isUndefined(core.languageProcessor.activeModel);
      assert.isFalse(core.languageProcessor.isActive);
      assert.isTrue(core.languageProcessor.mayPredict);
    });
  });

  describe('efficiency tests', function() {
    let testDistribution = [];
    var keyboard;
    let device = {
      formFactor: 'phone',
      OS: 'ios',
      browser: 'safari'
    }

    // Easy peasy long context:  use the input processor's full source!
    let coreSourceCode = fs.readFileSync('dist/index.js', 'utf-8');

    // At the time this test block was written...  810485 chars.
    // Let's force it to the same order of magnitude, even if the codebase grows.
    if(coreSourceCode._kmwLength() > 1000000) {
      coreSourceCode = coreSourceCode._kmwSubstring(0, 1000000);
    }

    this.beforeAll(function() {
      testDistribution = [];

      for(let c = 'A'.charCodeAt(0); c <= 'Z'.charCodeAt(0); c++) {
        let char = String.fromCharCode(c);

        testDistribution.push({
          keyId: "K_" + char,
          p: 1 / 26
        });
      }

      // Load the keyboard.  We'll need an InputProcessor instance as an intermediary.
      let core = new InputProcessor();

      // These two lines will load a keyboard from its file; headless-mode `registerKeyboard` will
      // automatically set the keyboard as active.
      let kbdScript = new vm.Script(fs.readFileSync('../tests/resources/keyboards/test_chirality.js'));
      kbdScript.runInThisContext();

      keyboard = core.activeKeyboard;
    });

    describe('without fat-fingering', function() {
      it('with minimal context (no fat-fingers)', function() {
        this.timeout(32); // ms
        let core = new InputProcessor();
        let context = new com.keyman.text.Mock("", 0);
  
        core.activeKeyboard = keyboard;
        let layout = keyboard.layout(com.keyman.utils.FormFactor.Phone);
        let key = layout.getLayer('default').getKey('K_A');
        let event = key.constructKeyEvent(core.keyboardProcessor, device);
        
        let behavior = core.processKeyEvent(event, context);
        assert.isNotNull(behavior);
      });
  
      it('with extremely long context (' + coreSourceCode._kmwLength() + ' chars, no fat-fingers)', function() {
        // Assumes no SMP chars in the source, which is fine.
        let context = new com.keyman.text.Mock(coreSourceCode, coreSourceCode._kmwLength());
  
        this.timeout(250); // 250 ms, excluding text import.
        let core = new InputProcessor();  // I mean, it IS long context, and time
                                          // thresholding is disabled within Node.
  
        core.activeKeyboard = keyboard;
        let layout = keyboard.layout(com.keyman.utils.FormFactor.Phone);
        let key = layout.getLayer('default').getKey('K_A');
        let event = key.constructKeyEvent(core.keyboardProcessor, device);
        
        let behavior = core.processKeyEvent(event, context);
        assert.isNotNull(behavior);
      });
    });

    describe('with fat-fingering', function() {
      it('with minimal context (with fat-fingers)', function() {
        this.timeout(32); // ms
        let core = new InputProcessor();
        let context = new com.keyman.text.Mock("", 0);
  
        core.activeKeyboard = keyboard;
        let layout = keyboard.layout(com.keyman.utils.FormFactor.Phone);
        let key = layout.getLayer('default').getKey('K_A');
        key.keyDistribution = testDistribution;
        let event = key.constructKeyEvent(core.keyboardProcessor, device);
        
        let behavior = core.processKeyEvent(event, context);
        assert.isNotNull(behavior);
      });
  
      it('with extremely long context (' + coreSourceCode._kmwLength() + ' chars, with fat-fingers)', function() {
        // Assumes no SMP chars in the source, which is fine.
        let context = new com.keyman.text.Mock(coreSourceCode, coreSourceCode._kmwLength());
  
        this.timeout(250); // 250 ms, excluding text import.
        let core = new InputProcessor();  // I mean, it IS long context, and time
                                          // thresholding is disabled within Node.
  
        core.activeKeyboard = keyboard;
        let layout = keyboard.layout(com.keyman.utils.FormFactor.Phone);
        let key = layout.getLayer('default').getKey('K_A');
        key.keyDistribution = testDistribution;
        let event = key.constructKeyEvent(core.keyboardProcessor, device);
        
        let behavior = core.processKeyEvent(event, context);
        assert.isNotNull(behavior);
      });
    });
  });
});