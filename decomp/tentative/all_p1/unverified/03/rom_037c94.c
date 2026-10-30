/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037c94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37c94(void)

{
  byte bVar1;
  char cVar2;
  int in_A0;
  int unaff_A6;
  undefined8 uVar3;

  if ((short)(*(short *)(in_A0 + 0x2c) - *(short *)(unaff_A6 + 0x2c)) < 1) {
    cVar2 = candidate_2eb4();
    bVar1 = cVar2 + 8U & 0xf0;
  }
  else {
    bVar1 = candidate_29c6();
    bVar1 = (bVar1 & 0x70) + 0x80;
  }
  *(byte *)(unaff_A6 + 0x96) = bVar1 >> 4;
  uVar3 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x30) = uVar3;
  return;
}
