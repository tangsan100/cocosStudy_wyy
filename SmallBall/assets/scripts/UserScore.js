
cc.Class({
    extends: cc.Component,

    properties: {
       scoreLabel:cc.Label,
        _score:0,
        score:{
            type:cc.Integer,
            get:function(){
                return this._score;
            },

            set:function(val){
                this._score = val;
                this.updateScoreLabel();
            }
        }
    },

   

    onLoad () {
       
    },

    init:function(){
        this.score = 0;
    },

    updateScoreLabel:function(){
        this.scoreLabel.string = this._score;
    },

    addScore:function(val){
        this.score += val;
    },

    start () {

    },
});
