{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {

  nativeBuildInputs = with pkgs; [
    # this is an extremely hacky way to compile raylib with custom frame control. Just adding the SUPPORT_CUSTOM_FRAME_CONTROL flag wouldn't work as the nix package uses older (stable or whatever) branch of raylib
    (raylib.overrideAttrs (oldAttrs: rec {
      src = builtins.fetchTarball {
        url = "https://github.com/raysan5/raylib/archive/master.tar.gz";
      };
      patches = [];
      cmakeFlags = [ "-DUSE_EXTERNAL_GLFW=ON" "-DBUILD_EXAMPLES=OFF" "-DCUSTOMIZE_BUILD=ON" "-DSUPPORT_CUSTOM_FRAME_CONTROL=ON" "-DINCLUDE_EVERYTHING=ON" "-DBUILD_SHARED_LIBS=ON" ];
    }))
    gdb
    gcc13
  ];
}
