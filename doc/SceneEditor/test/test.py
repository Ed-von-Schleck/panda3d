from ShowBaseGlobal import *
import Actor
class SavedScene:
    ModelDic={}
    ModelRefDic={}
    ActorDic={}
    ActorRefDic={}
    ActorAnimsDic={}
    def __init__(self):

        self.Model_1=loader.loadModel("test/badwizard1")
        self.Model_1.setX(0.0)
        self.Model_1.setY(0.0)
        self.Model_1.setZ(0.0)
        self.Model_1.setH(0.0)
        self.Model_1.setP(0.0)
        self.Model_1.setR(0.0)
        self.Model_1.setSx(1.0)
        self.Model_1.setSy(1.0)
        self.Model_1.setSz(1.0)
        self.Model_1.reparentTo(render)
        self.ModelDic['Model_1']=self.Model_1
        self.ModelRefDic['Model_1']=Filename.fromOsSpecific('E:/sshadhan/Desktop/DeskFolders/Eclipse/eclipse/workspace/SceneEditor/test/test/badwizard1.egg')
        self.ModelDic['Model_1'].setName('Model_1')

Scene=SavedScene()
run()
