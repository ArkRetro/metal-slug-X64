/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0722fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_722fe(void)

{
  char cVar1;
  int unaff_A6;
  undefined1 uVar2;

  *(short *)(unaff_A6 + 0x28) =
       (ushort)*(byte *)(unaff_A6 + 0x89) * 8 + 0x28 + *(short *)(unaff_A6 + 0x28);
  *(undefined4 *)(unaff_A6 + 0x30) = 0x80000;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  *(undefined1 *)(unaff_A6 + 0x8a) =
       *(undefined1 *)((short)(ushort)*(byte *)(unaff_A6 + 0x89) + 0x723ba);
  do {
    uVar2 = 0;
    candidate_1ac38();
    candidate_722b2();
    candidate_723c6();
    if ((bool)uVar2) {
      *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x8c) = 2;
      cVar1 = *(char *)(unaff_A6 + 0x89);
LAB_00072390:
      if (cVar1 != *(char *)(unaff_A6 + 0x84)) {
        *(char *)(unaff_A6 + 0x84) = cVar1;
        candidate_4256();
      }
      return;
    }
    if ((*(short *)(unaff_A6 + 0x3c) < 1) || (0x13f < *(short *)(unaff_A6 + 0x3c))) {
      cVar1 = *(char *)(unaff_A6 + 0x89);
      goto LAB_00072390;
    }
    if (*(byte *)(unaff_A6 + 0x8a) <= *(byte *)(unaff_A6 + 0x89)) {
      cVar1 = *(char *)(unaff_A6 + 0x89) + '\v';
      goto LAB_00072390;
    }
    *(byte *)(unaff_A6 + 0x89) = *(byte *)(unaff_A6 + 0x89) + 1;
  } while( true );
}
