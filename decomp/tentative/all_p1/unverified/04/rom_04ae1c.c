/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ae1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ae1c(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  byte bVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  if (*(char *)((int)unaff_A6 + 0x12) != -1) {
    *(char *)((int)unaff_A6 + 0x12) = *(char *)((int)unaff_A6 + 0x12) + '0';
    bVar4 = *(byte *)((int)unaff_A6 + 0x13);
    *(byte *)((int)unaff_A6 + 0x13) = bVar4 + 0x30;
    if (0xcf < bVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
      *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
    }
  }
  uVar3 = unaff_A6[0xd];
  uVar5 = CARRY4((int)-unaff_A6[0xd] >> 4,uVar3);
  unaff_A6[0xd] = ((int)-unaff_A6[0xd] >> 4) + uVar3;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_4e07a();
  return;
}
