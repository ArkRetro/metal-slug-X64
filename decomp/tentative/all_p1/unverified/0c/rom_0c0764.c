/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0764.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ulonglong candidate_c0764(void)

{
  ushort in_D0w;
  char cVar2;
  ushort uVar1;
  undefined1 extraout_D1b;
  undefined1 extraout_D1b_00;
  undefined1 uVar3;
  byte unaff_D2b;
  ushort *extraout_A0;
  int iVar4;
  int unaff_A5;

  uVar1 = in_D0w & 1;
  cVar2 = candidate_aaa88();
  if (cVar2 == '\0') {
SUB_000c07e0:
    return (ulonglong)
           CONCAT14(*(undefined1 *)(unaff_A5 + 0x4720 + (int)(short)(uVar1 & 1)),
                    (uint)*(byte *)(unaff_A5 + 0x4722 + (int)(short)(uVar1 & 1)));
  }
  if ((cVar2 == '\x01') || (cVar2 == '\b')) {
    uVar1 = candidate_ca2ce();
    uVar3 = extraout_D1b_00;
    if ((*extraout_A0 & 2) != 0) {
      iVar4 = unaff_A5 + -0x6b3c;
      if (uVar1 != 0) {
        iVar4 = unaff_A5 + -0x6a8c;
      }
      if ((*(byte *)(iVar4 + 0x15) & 0x40) != 0) goto SUB_000c07e0;
    }
  }
  else if ((((cVar2 == '\x02') || (cVar2 == '\x05')) || (cVar2 == '\x06')) ||
          (uVar3 = extraout_D1b, cVar2 == '\a')) goto SUB_000c07e0;
  return (ulonglong)CONCAT14(uVar3,(uint)unaff_D2b);
}
