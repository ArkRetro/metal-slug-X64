/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0a7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0a7c(void)

{
  ushort uVar1;
  ushort in_D0w;
  short in_D1w;
  int unaff_A5;
  ushort *puVar2;
  bool bVar3;
  ushort uStack_8;
  ushort uStack_6;
  undefined1 auStack_4 [4];

  if (in_D1w != 0) {
    bVar3 = false;
    uStack_8 = in_D0w;
    candidate_aa898();
    uVar1 = uStack_8;
    puVar2 = (ushort *)auStack_4;
    if (bVar3) {
LAB_000c0ac6:
      puVar2[-0xffffffff00000002] = 0xc;
      puVar2[-0xffffffff00000001] = 0xada;
      candidate_120f8();
      return;
    }
    if ((byte)uStack_6 < 4) {
      uStack_8 = uStack_6;
      candidate_c0b32();
      puVar2 = &uStack_8;
      if (*(char *)(unaff_A5 + 0x4722 + (int)(short)(uVar1 & 1)) != -0x6e) {
        *(undefined1 *)(unaff_A5 + 0x4722 + (int)(short)(uVar1 & 1)) = 0x92;
        goto LAB_000c0ac6;
      }
    }
  }
  return;
}
