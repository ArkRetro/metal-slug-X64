/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0383b4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_383b4(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_374ba();
  if (*(char *)((int)unaff_A6 + 0x97) == '\0') {
    candidate_4256();
    *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -8;
  }
  else {
    candidate_4256();
    sVar3 = -0x10;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      sVar3 = 0x10;
    }
    *(short *)(unaff_A6 + 10) = sVar3 + *(short *)(unaff_A6 + 10);
  }
  if (*(char *)((int)unaff_A6 + 0x9b) != '\0') {
    candidate_37c94();
  }
  *unaff_A6 = 0x38422;
  candidate_1ac38();
  iVar2 = *(int *)((int)unaff_A6 + 0x72);
  if (*(char *)(iVar2 + 0x9a) == '\0') {
    uVar4 = *(short *)(iVar2 + 0x28) - *(short *)(unaff_A6 + 10);
    if ((short)uVar4 < 0) {
      uVar4 = -uVar4;
    }
    if (uVar4 < 9) {
      uVar4 = *(short *)(iVar2 + 0x2c) - *(short *)(unaff_A6 + 0xb);
      if ((short)uVar4 < 0) {
        uVar4 = -uVar4;
      }
      if (uVar4 < 6) {
        *(byte *)(iVar2 + 0x5f) = *(byte *)(iVar2 + 0x5f) | 0x80;
        candidate_4126();
        iVar2 = *(int *)(unaff_A5 + -0x1c24);
        do {
          unaff_A6 = (undefined4 *)unaff_A6[2];
          pcVar1 = (code *)*unaff_A6;
          *(undefined4 *)(iVar2 + -4) = 0x424a;
          (*pcVar1)();
        } while( true );
      }
    }
  }
  candidate_37512();
  candidate_427a();
  return;
}
