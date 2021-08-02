var state = cc.Enum({
    Menu:-1,
    Start:-1,
    Over:-1
});

var Sheep = require ("Sheep");

var Game = cc.Class({
    extends: cc.Component,

    properties: {
       sheep:Sheep,
    },
    statics:{
        state
    },

    onLoad () {
        window.Global.GameManager = Game;
        this.state = state.Menu;
        

        // 小样初始化
        this.sheep.init();
    },

    start () {
        
        this.sheep.runStart(); 
    },

    // update (dt) {},
});
