/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046dce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46dce(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  char cVar4;
  undefined1 uVar5;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_29f2();
  candidate_446d4();
  *(undefined1 *)(unaff_A6 + 0x22) = 10;
  uVar5 = candidate_29c6();
  *(undefined1 *)((int)unaff_A6 + 0x87) = uVar5;
  *unaff_A6 = 0x46e16;
  candidate_446f0();
  candidate_1ac38();
  if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  bVar3 = *(byte *)((int)unaff_A6 + 0x87);
  *(byte *)((int)unaff_A6 + 0x87) = bVar3 - 0x40;
  if (bVar3 < 0x40) {
    candidate_40bc();
    candidate_1b220();
  }
  cVar4 = *(char *)(unaff_A6 + 0x22) + -1;
  *(char *)(unaff_A6 + 0x22) = cVar4;
  if (cVar4 == '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  }
  return;
}
