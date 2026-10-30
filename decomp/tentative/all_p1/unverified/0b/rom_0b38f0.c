/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b38f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b38f0(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ac38();
  if ((short)(*(short *)(unaff_A6 + 0x10) + -0x40) < 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_427a();
  unaff_A6[0xd] = ((int)(unaff_A6[0x1c] - unaff_A6[0xd]) >> 5) + unaff_A6[0xd];
  bVar3 = *(char *)((int)unaff_A6 + 0x6e) - *(char *)((int)unaff_A6 + 0x13);
  if ((bVar3 != 0) &&
     (*(char *)((int)unaff_A6 + 0x13) =
           *(char *)((int)unaff_A6 + 0x6f) + *(char *)((int)unaff_A6 + 0x13),
     (char)(bVar3 ^ *(char *)((int)unaff_A6 + 0x6e) - *(char *)((int)unaff_A6 + 0x13)) < '\0')) {
    *(undefined1 *)((int)unaff_A6 + 0x13) = *(undefined1 *)((int)unaff_A6 + 0x6e);
  }
  return;
}
