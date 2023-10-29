let
  nixpkgs = import <nixpkgs> {};
  pkgs = nixpkgs.pkgsCross.mingwW64;
in

# callPackage is needed due to https://github.com/NixOS/nixpkgs/pull/126844
pkgs.pkgsStatic.callPackage ({ mkShell, zlib, pkg-config, file }: mkShell {
  # these tools run on the build platform, but are configured to target the host platform
  nativeBuildInputs = [ pkg-config file ];
  # libraries needed for the host platform
  buildInputs = [ zlib ];
}) {}
