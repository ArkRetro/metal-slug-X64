/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027698.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_27698(void)

{
  int iVar1;
  ushort uVar2;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(short *)(unaff_A6 + 0x1e) = *(short *)(iVar1 + 0x1e) + (short)*(char *)(unaff_A6 + 0x8b);
  uVar2 = *(byte *)(iVar1 + 0x84) & 3;
  *(ushort *)(unaff_A6 + 0x28) =
       *(short *)(unaff_A6 + 0x7a) + (short)*(char *)(unaff_A6 + 0x89) * uVar2;
  *(ushort *)(unaff_A6 + 0x2c) =
       *(short *)(unaff_A6 + 0x7c) + (short)*(char *)(unaff_A6 + 0x8a) * uVar2;
  if ((*(byte *)(unaff_A6 + 0x8c) & 1) != 0) {
    *(undefined1 *)(unaff_A6 + 0x14) =
         *(undefined1 *)(unaff_A6 + 0x19 + (int)(short)(*(byte *)(iVar1 + 0x8c) & 3));
  }
  if ((*(byte *)(unaff_A6 + 0x8c) & 2) != 0) {
    if ((_DAT_0010000e & 2) == 0) {
      *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1a);
    }
  }
  if (*(char *)(unaff_A6 + 0x87) == '\0') {
    candidate_44f6();
    *(undefined2 *)(unaff_A6 + 0x66) = 0;
    *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  }
  return;
}
