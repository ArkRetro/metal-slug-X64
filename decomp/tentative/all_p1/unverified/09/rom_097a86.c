/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097a86.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_97a86(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 in_D0w;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(byte *)((int)unaff_A6 + 0x79) =
       (byte)(CONCAT11((char)((ushort)in_D0w >> 8),*(undefined1 *)((int)unaff_A6 + 0x6e)) >> 3) &
       0x1f;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_97b18;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) goto LAB_00097ae6;
  uVar3 = candidate_49e8();
  uVar4 = uVar3 < 6;
  if (!(bool)uVar4) goto LAB_00097aec;
  uVar4 = CARRY2(uVar3 * 2,uVar3 * 2);
                    /* WARNING: Switch is manually overridden */
  switch(uVar3 * 4) {
  case 0:
    break;
  case 4:
    break;
  case 8:
    goto LAB_00097ae6;
  case 0xc:
LAB_00097ae6:
    *unaff_A6 = candidate_97b02;
    uVar4 = false;
    break;
  case 0x10:
    break;
  case 0x14:
  }
LAB_00097aec:
  candidate_427a();
  candidate_19504();
  if (!(bool)uVar4) {
    return;
  }
  candidate_97b24();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
