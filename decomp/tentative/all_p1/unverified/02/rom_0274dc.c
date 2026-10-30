/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0274dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_274dc(void)

{
  byte bVar2;
  short sVar1;
  char cVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_1b04a();
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x80) == 0) {
    bVar2 = candidate_1b1c0();
  }
  else {
    bVar2 = candidate_1b14a();
  }
  *(byte *)((int)unaff_A6 + 0x86) = bVar2 & 0xf8;
  *(byte *)((int)unaff_A6 + 0x85) = *(byte *)((int)unaff_A6 + 0x85) & 0xf8;
  if (-1 < *(short *)(unaff_A6 + 0xc)) {
    *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 1;
  }
  *(undefined2 *)(unaff_A6 + 0x1d) = 8;
  *unaff_A6 = 0x27530;
  cVar3 = -8;
  bVar2 = *(byte *)((int)unaff_A6 + 0x86);
  if ((bVar2 & 1) != 0) {
    cVar3 = '\b';
    bVar2 = bVar2 & 0xfe;
  }
  if (bVar2 != *(byte *)((int)unaff_A6 + 0x85)) {
    *(char *)((int)unaff_A6 + 0x85) = cVar3 + *(char *)((int)unaff_A6 + 0x85);
  }
  candidate_27820();
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_27576;
  }
  candidate_1abc8();
  candidate_1ac38();
  *(byte *)(unaff_A6 + 0x21) = (byte)(*(char *)((int)unaff_A6 + 0x85) + 4U) >> 3;
  candidate_427a();
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  if ('\x02' < *(char *)(unaff_A6 + 0x23)) {
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)((int)unaff_A6 + (short)(*(byte *)(unaff_A6 + 0x23) & 3) + 0x19);
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_27616;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_27616;
  }
  uVar4 = false;
  candidate_19424();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_27616;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
