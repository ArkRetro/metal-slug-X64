/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06bd46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6bd46(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  sVar1 = *(short *)(unaff_A6 + 0xf) - *(short *)(unaff_A6 + 0x22);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    sVar1 = -sVar1;
  }
  uVar2 = 0;
  if (sVar1 < 1) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
    *unaff_A6 = 0x6bd96;
    uVar2 = 0;
    candidate_1ac38();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_6bdb0;
      uVar2 = 0;
    }
  }
  else {
    candidate_6ca5c();
    candidate_1ac38();
    candidate_427a();
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6c0d8;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6c14c;
  }
  candidate_140c0();
  return;
}
