
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
        },
        lineSound:{
            type:cc.AudioClip,
            default:null
        }
        
    },

    // LIFE-CYCLE CALLBACKS:

    onLoad () {
        window.Level = this;
    },

    init:function(bgNodeControll){
        this.bgNodeControll = bgNodeControll;
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
                node.isHit = false;
            }
        });
    },

    onCollision(tag,node){
        if (!node.isHit){
            this.curNum++;
            node.isHit = true;
        }
            

        if (tag == 6){
            this.playLineEffect(tag,node)
        }else {
            this.playBlockEffect(tag,node);
        }

    },

    playLineEffect:function(tag,node){
        // 播放动画
        var ani = node.getComponent(cc.Animation);
        ani.play("line")
        cc.audioEngine.play(this.lineSound,false,1);
    },

    playBlockEffect:function(tag,node){

         // 播放音效
         this.playSound();

        // 创建背景
        node.active = false;
        var bg  = this.bgNodeControll.createBgNode(node.parent)//cc.instantiate(this.blockBgPrefab);

        // 更换背景资源
        var bgSpr = bg.getComponent(cc.Sprite);
        bgSpr.spriteFrame = this.blockBgRes[tag];

        // 处理背景的位置信息等
        bg.position = node.position;
        bg.isBg = true;
        bg.angle = node.angle
        bg.scaleX = node.scaleX;
        bg.scaleY = node.scaleY;
        bg.opacity = 255



        // 动作处理
        var scale = cc.scaleBy(0.5,1.2,1.2);
        var fade = cc.fadeOut(0.5);
        var spawn = cc.spawn(scale,fade);
        var func = cc.callFunc(function(){
            // bg.removeFromParent();
            this.bgNodeControll.onDestroyBgNode(bg);
        }.bind(this))

        bg.runAction(cc.sequence(spawn,func));
    },

    onCollisionBlack:function(tag,node){

        if (!node.oldScaleX){
            node.oldScaleX = node.scaleX;
            node.oldScaleY = node.scaleY;
        }

        node.scaleX = node.oldScaleX;
        node.scaleY = node.oldScaleY

        node.stopAllActions();

        var scaleBig = cc.scaleBy(0.1,1.1);
        var scaleSmall = cc.scaleBy(0.1,0.909)
        node.runAction(cc.sequence(scaleBig,scaleSmall))
        cc.audioEngine.play(this.blackSound,false,1);
    },

    start () {

    },

    update (dt) {
    
    },
});
