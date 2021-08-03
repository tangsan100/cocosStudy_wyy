function onCollisionEnter(other,self){
    this.proxyComp.onSheepCollision(other,self);
}

cc.Class({
    extends: cc.Component,

    properties: {
        // 做代理转发
        proxyComp:cc.Component
     
    },

   

    onLoad () {
        // this.onCollisionEnter=null;
        // this.onCollisionStay=null;
        // this.onCollisionExit=null;
        this.proxyComp = this.node.parent.getComponent("Sheep");
        
    },

    start () {
        // cc.log(this.proxyComp);
        // if (this.proxyComp){
        //     this.onCollisionEnter=function (other, self) {
        //         cc.log(this.proxyComp);
        //         this.proxyComp.onSheepCollision(other,self);
                
        //     }.bind(this);
        // }
    },

    // 代理转发
    onCollisionEnter: function (other, self) {
        if (this.proxyComp){
            this.proxyComp.onSheepCollision(other,self);
        }
    }

    // update (dt) {},
});
