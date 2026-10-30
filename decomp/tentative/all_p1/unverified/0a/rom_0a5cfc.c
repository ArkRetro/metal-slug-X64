/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5cfc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5cfc(void)

{
  byte bVar1;
  undefined2 extraout_D0u;
  uint uVar2;
  byte *pbVar3;
  int unaff_A6;

  func_0x000a5caa();
  func_0x000a6d7c();
  uVar2 = CONCAT22(extraout_D0u,*(undefined2 *)(unaff_A6 + 0x74));
  pbVar3 = (byte *)0x2a6355;
  bVar1 = bRam002a6354;
  while (bVar1 != 0xff) {
    if ((uVar2 & 1 << (bVar1 & 0x1f)) != 0) {
      func_0x000a6d7c(uVar2,(uint)bVar1);
    }
    bVar1 = *pbVar3;
    pbVar3 = pbVar3 + 1;
  }
  return;
}
