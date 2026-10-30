/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d726e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d726e(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  int unaff_A6;
  bool bVar2;

  if ((((*(char *)(unaff_A6 + 0x7a) != '\x05') &&
       (*(undefined1 *)(unaff_A6 + 0xa2) = 0, '\x04' < *(char *)(*(int *)(unaff_A6 + 0xc) + 0x79)))
      && (iVar1 = *(int *)(unaff_A6 + 0xc), *(char *)(iVar1 + 0x79) != '\x02')) &&
     (*(char *)(iVar1 + 0x79) != '\x03')) {
    if (*(char *)(iVar1 + 0x79) == '\x04') {
      *(undefined1 *)(unaff_A6 + 0xa2) = 0;
    }
    bVar2 = false;
    candidate_40bc();
    iVar1 = extraout_A0;
    if (bVar2) {
      candidate_1b220();
      iVar1 = extraout_A0_00;
    }
    *(undefined1 *)(iVar1 + 0xa4) = *(undefined1 *)(unaff_A6 + 0xa2);
    *(undefined1 *)(iVar1 + 0x7d) = *(undefined1 *)(unaff_A6 + 0x7d);
    *(char *)(unaff_A6 + 0x7d) = *(char *)(unaff_A6 + 0x7d) + '\x01';
  }
  return;
}
