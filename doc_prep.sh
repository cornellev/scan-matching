make docs
mv docs ..
git stash
git checkout docs
rm -rf book .vscode src math script install ex_data Doxyfile .clang-format AUTHOR.md INSTALL.md main.cpp Makefile README.md README.md.build sim.conf test.cpp
mv ../docs .
git add .
git commit -m "Update docs"
git push origin docs
git checkout main
