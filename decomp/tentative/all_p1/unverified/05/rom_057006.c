/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057006.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_57006(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  ushort extraout_D1w;
  uint *puVar4;
  uint *puVar5;
  int unaff_A6;

  uVar3 = 0xffff;
  sVar2 = -1;
  if ((&DAT_0010fdb6)[*(short *)(unaff_A6 + 0x74)] != '\0') {
    puVar4 = (uint *)&DAT_0010c46a;
    puVar5 = (uint *)&DAT_0010c472;
    if (*(short *)(unaff_A6 + 0x74) != 0) {
      puVar4 = (uint *)&DAT_0010c472;
      puVar5 = (uint *)&DAT_0010c46a;
    }
    sVar1 = candidate_57b44();
    sVar2 = sVar1 + -1;
    uVar3 = extraout_D1w;
    if (((extraout_D1w < 10) &&
        ((&DAT_0010fdb6)[(short)(ushort)(*(short *)(unaff_A6 + 0x74) == 0)] != '\0')) &&
       (*puVar4 <= *puVar5)) {
      if (*puVar5 == *puVar4) {
        if (*(short *)(unaff_A6 + 0x74) != 0) {
          uVar3 = extraout_D1w + 1;
        }
      }
      else {
        uVar3 = extraout_D1w + 1;
        sVar2 = sVar1;
      }
    }
  }
  if (9 < uVar3) {
    sVar2 = 999;
  }
  return sVar2;
}
