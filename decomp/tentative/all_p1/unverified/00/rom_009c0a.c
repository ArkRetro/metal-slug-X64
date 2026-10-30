/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009c0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9c0a(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short in_D0w;
  short in_D1w;
  short unaff_D2w;
  byte unaff_D3b;
  ushort uVar4;

  if (in_D0w < 0x20) {
    if (in_D0w < -0x1f) {
      in_D0w = -0x1f;
    }
  }
  else {
    in_D0w = 0x1f;
  }
  if (in_D1w < 0x20) {
    if (in_D1w < -0x1f) {
      in_D1w = -0x1f;
    }
  }
  else {
    in_D1w = 0x1f;
  }
  if (unaff_D2w < 0x20) {
    if (unaff_D2w < -0x1f) {
      unaff_D2w = -0x1f;
    }
  }
  else {
    unaff_D2w = 0x1f;
  }
  sRam00109920 = in_D0w * 0x100;
  sRam00109922 = in_D1w * 0x100;
  sRam00109924 = unaff_D2w * 0x100;
  if (unaff_D3b < 2) {
    unaff_D3b = 1;
  }
  bRam0010992c = unaff_D3b;
  sVar1 = in_D0w * 0x100 - sRam0010991a;
  sVar2 = in_D1w * 0x100 - sRam0010991c;
  sVar3 = unaff_D2w * 0x100 - sRam0010991e;
  if (sVar3 == 0 && (sVar2 == 0 && sVar1 == 0)) {
    bRam0010992c = 0;
    return;
  }
  if (unaff_D3b != 0) {
    uVar4 = (ushort)unaff_D3b;
    sRam00109926 = sVar1 / (short)uVar4;
    sRam00109928 = sVar2 / (short)uVar4;
    sRam0010992a = sVar3 / (short)uVar4;
  }
  return;
}
