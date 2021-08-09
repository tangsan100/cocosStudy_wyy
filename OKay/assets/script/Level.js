
cc.Class({
    extends: cc.Component,

    properties: {
        blockBgRes:{
            default:[],
            type:[cc.SpriteFrame],
        },
        blockBgPrefab:cc.Prefab,
        levelAni:cc.Prefab,
        sounds:{
            type:[cc.AudioClip],
            default:[]
        },

        blackSound:{
            type:cc.AudioClip,
            default:null
        }
        
    },

    // LIFE-CYCLE CALLBACKS:

    onLoad () {
        window.Level = this;
    },

    /*
    * 设置新的关卡
    * level 新的关卡编号
    */
    setLevel:function(level){
        this.currentLevel = level;
        this.childNode = this.node.getChildByName(''+level)
        this.childNode.active = true;
       
        var blockNum = this.childNode.children.length;

        this.childNode.children.forEach(node => {
            if (node.getComponent("Black")){
                blockNum --;
            }
        });
        this.blockNum = blockNum;
        this.curNum = 0;

    },

    playSound:function(){
        var index = this.curNum -1;
        if (index > 7){
            index = 7
        }

        cc.audioEngine.play(this.sounds[index],false,1);
    },

    processEnd(){
        if (this.curNum >= this.blockNum){
            // 通关
            var levelAni = cc.instantiate(this.levelAni)
            levelAni.getComponent("LevelAni").setLevel(this.currentLevel,this.currentLevel+1);
            levelAni.parent = this.node;
            this.childNode.active = false;
        }else {
            // 恢复原样
            this.reset();
        }
    },


    // 恢复原样
    reset:function(){
        this.curNum = 0;
        this.childNode.children.forEach(node => {
            if (!node.isBg){
                node.active = true;
            }
        });
    },

    onCollision(tag,node){
    
        this.curNum++;
        node.active = false;
        var bg  = cc.instantiate(this.blockBgPrefab);
        var bgSpr = bg.getComponent(cc.Sprite);
        bgSpr.spriteFrame = this.blockBgRes[tag];

        bg.parent = node.parent;
        bg.position = node.position;
        bg.isBg = true;
        bg.angle = node.angle
        bg.scaleX = node.scaleX;
        bg.scaleY = node.scaleY;
      


        // 动作处理
        var scale = cc.scaleBy(0.5,1.2,1.2);
        var fade = cc.fadeOut(0.5);
        var spawn = cc.spawn(scale,fade);
        var func = cc.callFunc(function(){
            bg.removeFromParent();
        }.bind(bg))

        bg.runAction(cc.sequence(spawn,func));

        this.playSound();
    },

    onCollisionBlack:function(tag,node){
        cc.audioEngine.play(this.blackSound,false,1);
    },

    start () {

    },

    update (dt) {
    
    },
});
