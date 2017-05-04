/* 
 * File:   Optimizer.h
 * Author: len
 *
 * Created on January 13, 2016, 3:15 PM
 */

#ifndef OPTIMIZER_H
#define	OPTIMIZER_H


namespace Cool {
    class AsmContainer;
    namespace Assembly {
        class Optimizer {
        public:
            void optimize(AsmContainer & container);
        };
    }
}
#endif	/* OPTIMIZER_H */

