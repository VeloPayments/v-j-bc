.PHONY: ALL vcblockchain-build vcblockchain-test vcblockchain-clean
.PHONY: mvn-compile mvn-install mvn-package mvn-clean model-check clean

MODEL_MAKEFILES?= \
	$(foreach file,\
	    $(wildcard vjblockchain-native/models/*.mk),$(notdir $(file)))

ALL: mvn-install model-check

mvn-install: mvn-package
	(cd vjblockchain-java && mvn install)

mvn-package: mvn-compile
	mvn package

mvn-compile: vcblockchain-test
	mvn compile

mvn-clean:
	mvn clean

model-check:
	for n in $(MODEL_MAKEFILES); do \
	    (cd vjblockchain-native/models && $(MAKE) -f $$n) \
	done

clean: vcblockchain-clean

vcblockchain-clean: mvn-clean
	(cd lib/vcblockchain && $(MAKE) clean)

vcblockchain-test: vcblockchain-build
	(cd lib/vcblockchain && $(MAKE) test)

vcblockchain-build:
	(cd lib/vcblockchain && $(MAKE))
