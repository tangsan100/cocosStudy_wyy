
cc.Class({
    extends: cc.Component,

    properties: {
        speed:0,
        resetX:0,
    },

    
    // onLoad () {},

    start () {

    },

    update (dt) {
        // 获取当前x 坐标
        var x = this.node.x;
        // 获得本次的偏移量
        x += dt*this.speed;

        // 过界判断
        if (x <= this.resetX){
            x -= this.resetX;
        }

        this.node.x = x;
    },
});
