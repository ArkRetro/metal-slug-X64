/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043ee0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43ee0(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  undefined8 uVar5;

  uVar5 = candidate_3b332();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar5;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0x6000;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 0;
  *unaff_A6 = 0x43f1a;
  uVar3 = unaff_A6[0xd];
  uVar4 = CARRY4(*(uint *)((int)unaff_A6 + 0x76),uVar3);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x76) + uVar3;
  candidate_1ac38();
  candidate_427a();
  candidate_1945c();
  if (!(bool)uVar4) {
    candidate_4c24();
    if ((bool)uVar4) {
      *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) | 6;
      if (*(char *)((int)unaff_A6 + 0x5a) == '\f') {
        *unaff_A6 = candidate_43f32;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        *unaff_A6 = candidate_43f32;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if (*(char *)(unaff_A6 + 0x27) != '\0') {
      *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
    }
    candidate_446f0();
    return;
  }
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
