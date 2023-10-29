#!/usr/bin/env bb

(require '[clojure.string :as str])
(require '[babashka.process :as p])
(require '[babashka.fs :as fs])

(def cc "x86_64-w64-mingw32-gcc")

(defn get-c-files [src-dir]
  (:out (p/shell {:out :string :continue true} "find" src-dir "-name" "*.c")))

(defn to-o [src-files]
  (let [o-files (map ;#(str "./out" (subs % (count "./src")))
                   #(str (->> (subs % (count "./src"))
                              (str "./out")
                              (fs/split-ext)
                              (first)) ".o")
                   src-files)]
    (doall
      (map (fn [[c o]]
             (p/shell cc c "-c" "-o" o "-I" "./raylib/src/" "-L" "./lib/" "-lraylib" "-lm" "-lwinmm" "-lgdi32"))
           (zipmap src-files o-files)))
    o-files))

(defn to-exe [o-files name]
  (apply p/shell (flatten [cc o-files "-o" name "-I" "./raylib/src/" "-L" "./lib/" "-lraylib" "-lm" "-lwinmm" "-lgdi32"])))

(let [c-files (str/split (get-c-files "./src") #"\n")]
  (to-exe (to-o c-files) "out/quandeth.exe"))
