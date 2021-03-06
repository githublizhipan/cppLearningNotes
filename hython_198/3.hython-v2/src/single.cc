#include <single.h>
namespace haizei {

const IHandle *MasterChainSingle::object = nullptr;
std::mutex Singleton::m_mutex;

int MasterChainSingle::run(ASTNode node, std::shared_ptr<Parameter> &p) {
    const IHandle *h = get();
    int ret = 0;
    while (h != nullptr) {
        if (!(h->Test(node))) {
            h = h->next();
            continue;
        }
        //调用对应 master 的 run 方法
        auto m = h->getMaster();
        ret = m->run(node, p);
        break;
    }
    if (h == nullptr) throw std::runtime_error(
        "not handle type : "
        + std::to_string(node.type())
    );
    return ret;
}



const IHandle *MasterChainSingle::get() {
    //加锁的懒汉模式
    if (object == nullptr) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (object == nullptr) {
            object = SingleInit();
        }
    }
    return object;
}
const IHandle *MasterChainSingle::SingleInit() {
    PrintHandle *node4 = new PrintHandle(nullptr);
    ControNoBlockHandle *node3 = new ControNoBlockHandle(node4);
    ControBlockHandle *node2 = new ControBlockHandle(node3);
    ExprHandle *node1 = new ExprHandle(node2);
    return node1;
}



}
